// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// grvy_ramdisk: Routines to implement ramdisk over MPI
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy_classes.h>
#include<grvy.h>
#include<grvy_int.h>
#include<map>

using namespace std;
using namespace GRVY;

#define info  GRVY_INFO
#define warn  GRVY_WARN
#define error GRVY_ERROR
#define debug GRVY_DEBUG
#define prefix "mpi_ocore"

typedef struct MPI_Ocore_datagram {
  bool in_mem;			// data resides in memory or disk?
  size_t read_counts;		// read  frequency
  size_t write_counts;		// write frequency
  size_t index;			// data index location (for mem or disk)
} MPI_Ocore_datagram;

//------------------------------------------------------------
// private implementation class definition for GRVY_MPI_Ocore
//------------------------------------------------------------

namespace GRVY {

  class GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp 
  {
  public:
    GRVY_MPI_Ocore_ClassImp () {}
   ~GRVY_MPI_Ocore_ClassImp () {}
    
    int Initialize(string input_file, int blocksize);
    int GetRecord (int sparse_index);
  
    bool use_mpi_ocore;
    bool initialized;			
    bool master;			// master MPI process
    string inputfile;			// input file with runtime controls
    int max_poolsize_MBs;		// max ramdisk size (in MBs)
    int max_mapsize_MBs;		// max mapsize (in MBs)
    int word_size;			// individual word size in bytes
    int blocksize;			// data element blocksize 
    int mpi_rank;			// rank of local processs
    int mpi_nprocs;			// total number of procs available
    int sendtag;		        // MPI message sendtag
    int recvtag;			// MPI message recvtag
    int num_map_records;		// max number of mappable record indices
    int max_num_records;		// max number of raw records
    int num_active;		        // active number of raw records

    vector< vector <double>  > pool;    // raw data pool storage
    map<int,int> smap;			// sparse data map (sparse indices -> contiguous pool indices)
    //vector<MPI_Ocore_datagram> map;	// data map

    GRVY_MPI_Ocore_Class *self;	        // back pointer to public class

  private:
    int num_active_records;	        // number of currently active records
    
};

} // matches namespace GRVY

namespace GRVY {

  GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_Class() :m_pimpl(new GRVY_MPI_Ocore_ClassImp() )
  {

    m_pimpl->initialized           = false;

    // Default settings 

    m_pimpl->use_mpi_ocore         = true;
    m_pimpl->max_mapsize_MBs       = 10;
    m_pimpl->max_poolsize_MBs      = 200;
    m_pimpl->word_size             = 8;
    m_pimpl->blocksize             = 8192;
    m_pimpl->sendtag               = 1001;
    m_pimpl->recvtag               = 2001;
    m_pimpl->self                  = this;


    //------------
    // MPI setup
    //------------

    // temporary

    m_pimpl->mpi_rank   = 0;
    m_pimpl->mpi_nprocs = 1;

    if(m_pimpl->mpi_rank == 0)
      m_pimpl->master = true;

    // set default options for ocore

//    m_pimpl->options["output_stdout"        ] = true;
//    m_pimpl->options["output_totaltimer_raw"] = true;

  }

  GRVY_MPI_Ocore_Class::~GRVY_MPI_Ocore_Class()
  {
    // using auto_ptr for proper cleanup
  }

  int GRVY_MPI_Ocore_Class::Initialize(string input_file,int blocksize)
  {
    return(m_pimpl->Initialize(input_file,blocksize));
  }

  int GRVY_MPI_Ocore_Class::Write(int rank, int size, int offset, double *data)
  {
    
    return(m_pimpl->Initialize(input_file,blocksize));
  }

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Initialize(string input_file,int blocksize)
  {

    if(!use_mpi_ocore) return 0;

    if(master)
      grvy_printf(info,"\n%s: Initializing on %i processors...\n",prefix,mpi_nprocs);

    // todo: read input variables

    // todo: bcast to children

    num_map_records = max_poolsize_MBs*1024*1024/sizeof(int);
    max_num_records     = max_poolsize_MBs*1024*1024/(blocksize*word_size);

    if(master)
      {
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Individual record word size        = %15i (Bytes )\n",prefix,word_size);
	grvy_printf(info,"%s: --> Individual record blocksize        = %15i (Bytes )\n",prefix,blocksize);
	grvy_printf(info,"%s: --> Max sparse map memory size         = %15i (MBytes)\n",prefix,max_mapsize_MBs);
	grvy_printf(info,"%s: --> MPI ramdisk memory size (per proc) = %15i (MBytes)\n",prefix,max_poolsize_MBs);
	grvy_printf(info,"%s: --> Max number of mappable records     = %15i\n",         prefix,num_map_records);
	grvy_printf(info,"%s: --> Max number of raw ramdisk records  = %15i\n",         prefix,max_num_records);
	grvy_printf(info,"\n");
      }
    
    // -------------------------------
    // allocation for raw storage pool
    // -------------------------------

#if 0    
    try 
      {
	vector<double> data(blocksize);
	
	for(int i=0;i<max_num_records;i++)
	  pool.push_back(data);
      }
	
    catch(const std::bad_alloc& ex) 
      {
	grvy_printf(error,"%s (p%5i): Unable to allocate raw storage pool...exiting.\n",prefix,mpi_rank);
	exit(1);
      }
      
    catch(...)
      {
	grvy_printf(error,"%s (p%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	exit(1);
      }
	
    grvy_printf(info,"%s (%5i): Successfully initialized ramdisk storage pool of %8i MBs\n",
		prefix,mpi_rank,max_poolsize_MBs);
#endif

    initialized = true;    
    return 0;
  }

  // --------------------------------------------------------------------------------
  // GetRecord(): map sparse index to Ocore index
  // --------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: GetRecord(int sparse_index)
  {

    map<int,int>:: iterator it = smap.find(sparse_index);

    if(it == smap.end())	// new record
      {
	// store mapping and allocate raw storage for new record

	try {
	  smap[sparse_index] = ++num_active_records;;

	  vector<double> data(blocksize);
	  pool.push_back(data);
	}

	catch(const std::bad_alloc& ex) 
	  {
	    grvy_printf(error,"%s (%5i): Unable to allocate raw storage pool...exiting.\n",prefix,mpi_rank);
	    exit(1);
	  }

	catch(...)
	  {
	    grvy_printf(error,"%s (%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	    exit(1);
	  }

	grvy_printf(info,"%s (%5i): Registered new active record (record = %15i)\n",prefix,num_active_records);

	return(num_active_records);
      }
    else			// an old record
      return( it->second );
  }

} // matches namespace GRVY