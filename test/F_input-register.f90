!*----------------------------------------------
! * GRVY Test Suite
! *
! * Input variable registration
! *
! * $Id: input-register.f90 2422 2010-03-09 19:33:42Z nick
! *----------------------------------------------

program main
  use grvy
  implicit none

  integer(4)        :: flag
  character(len=60) :: filepath

  ! to be written
  integer(4)        :: iset = 123
  real(4)           :: fset = 456.7
  real(8)           :: dset = 123.e10
  character(len=60) :: sset = "I am the very model of a modern Major-General"

  ! to be read
  integer(4)        :: igot
  real(4)           :: fgot 
  real(8)           :: dgot 
  character(len=60) :: sgot

  ! Register variable (used for backwards-compatible input file
  ! support)

  call grvy_input_register_int   ("sec1/aint",iset,flag)
  call grvy_input_register_float ("sec1/afloat", fset,flag)
  call grvy_input_register_double("sec1/adouble",dset,flag)
  call grvy_input_register_char  ("sec1/astring",sset,flag)

  ! Verify that we can recover what we registered as default

  call grvy_input_register_get_int   ("sec1/aint",igot,flag)
  call grvy_input_register_get_float ("sec1/afloat",fgot,flag)
  call grvy_input_register_get_double("sec1/adouble",dgot,flag)
  call grvy_input_register_get_char  ("sec1/astring",sgot,flag)

  if(flag.eq.0) then
     call exit(1)
  endif

  if(igot .ne. iset .or. fgot .ne. fset .or. dgot .ne. dset) then
     call exit(1)
  endif

  ! Compare character string

  if ( len_trim(sset) .ne. len_trim(sgot) ) then
     call exit(1)
  endif

  if( sset .ne. sgot )then
     call exit(1)
  endif

  ! Verify that we can recover registered default variables
  ! even if they are not present in the input file

  call grvy_log_setlevel(GRVY_NOLOG)

  igot=-1
  fgot=-1.
  dgot=-1.0d0
  sgot= ""

  ! TODO: need to build a Fortran equivalent of
  ! build_example_file_path() that is used in C tests to support VPATH
  ! builds - might want to use ISO_C bindings to call the C version
  ! directly.  For now, revert to using local copy of input file.

  !call build_example_file_path    (filepath,flag)
  !call grvy_input_fopen           (filepath,flag)

  call grvy_input_fopen       ("./input-example.txt",flag)
  call grvy_input_fread_int   ("sec1/aint",igot,flag)
  call grvy_input_fread_real  ("sec1/afloat",fgot,flag)
  call grvy_input_fread_double("sec1/adouble",dgot,flag)
  call grvy_input_fread_char  ("sec1/astring",sgot,flag)
  call grvy_input_fclose      ()

  if(flag .eq. 0) then
     call exit(1);
  endif

  if(igot .ne. iset .or. fgot .ne. fset .or. dgot .ne. dset) then  
     call exit(1);
  endif

  ! Compare character string

  if ( len_trim(sset) .ne. len_trim(sgot) ) then
     call exit(1)
  endif

  if( sset .ne. sgot )then
     call exit(1)
  endif

  call exit(0)
  
end program main