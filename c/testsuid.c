
int
main()
{
     int status = 0;
     setuid(1);
     if(
#ifdef HAVESETEUID
        seteuid(0) != -1 ||
#endif
#ifdef HAVESETRUID
        setruid(0) != -1 ||
#endif
	setuid(0) != -1)
        status = 1;
     exit(status);
}
