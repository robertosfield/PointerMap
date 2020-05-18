Utility program for converting "0xfffffffff" style points in text files/streams to more human radable "pointer_#" form i.e.


   vsgdraw -d -a | pointermapper

The resulting output is more human readable and easier to use with diff tools for comparing different runs of applications, such as when looking at before and after results from changes to application/library.  The numbering of pointers is done from 0 onwards.  So first 0xffffffff pointer encountered in input is converted to "pointer_0" and so on.