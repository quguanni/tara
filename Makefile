########################### KUANG ##########################
#
#	make all	    -- makes everything
#	make <program_name> -- make a given program

EXECUTABLE = addto clearfiles filewriters members tilde \
		k_server k_client add_R_users process_success_files  

C_SRC      = addto.c clearfiles.c filewriters.c members.c tilde.c 

CFLAGS     = -O $(C2)


#
# Where the programs are....
#
CHMOD=/bin/chmod
TEST=/usr/bin/test
MKDIR=/bin/mkdir
CP=/bin/cp
CC=/usr/bin/cc
RM=/bin/rm

# make default
default:	$(EXECUTABLE)

# make all
all:	$(EXECUTABLE)


# <program_name>
####################################################

addto: src/addto.c
	$(CC) $(CFLAGS) -o addto src/addto.c

clearfiles: src/clearfiles.c
	$(CC) $(CFLAGS) -o clearfiles src/clearfiles.c

filewriters: src/filewriters.c
	$(CC) $(CFLAGS) -o filewriters src/filewriters.c

members: src/members.c
	$(CC) $(CFLAGS) -o members src/members.c


tilde: src/tilde.c
	$(CC) $(CFLAGS) -o tilde src/tilde.c

####################################################


k_server: server.c 
	gcc -o k_server -g server.c

k_client: client.c
	gcc -o k_client -g client.c


add_R_users: add_R_users.c
	gcc -g -o add_R_users add_R_users.c


####################################################

process_success_files: process_success_files.c
	gcc -o process_success_files -g process_success_files.c

####################################################

clean:
	$(RM) -f $(EXECUTABLE) Success* 




########################### TIGER ##########################

#
#  You only need to 'make' tiger if you are planning to run it
#  on a regular basis.
#
#
#------------------------------------------------------------------------
#
# This directory will contain the 'tiger', 'tigercron', 'tigexp'
# scripts, config files, the 'scripts' subdirectory which will
# contain the checking scripts, the platform specific scripts, etc.
# None of this will need to be writable once installed.
#
TIGERHOME=/usr/local/tiger
#
# This directory is used for scratch files while the scripts
# are running.  It can be /tmp.  By using something other
# than /tmp (something used only by the 'tiger' scripts), you
# can easily clean up the occasional dribbles left by 'tiger'
# (report these... don't want anything left laying around).
# 
# Of course, it is necessary that this directory be writable.
#
TIGERWORK=/usr/spool/tiger/work
#
# Where do log files go.  This directory must be writable.
#
TIGERLOGS=/usr/spool/tiger/logs
#
# Where do binary executables go... this is only used if the
# binary executables don't exist in the appropriate platform
# sub-directory under $(TIGERHOME)/systems
#
TIGERBIN=/usr/spool/tiger/bin
#
# End of user customization...
#
#------------------------------------------------------------------------
#
SCRIPTS=./tiger \
	./tigercron \
	./tigexp \
	./scripts/check_accounts \
	./scripts/check_aliases \
	./scripts/check_anonftp \
	./scripts/check_cron \
	./scripts/check_embedded \
	./scripts/check_exports \
	./scripts/check_group \
	./scripts/check_inetd \
	./scripts/check_known \
	./scripts/check_netrc \
	./scripts/check_passwd \
	./scripts/check_path \
	./scripts/check_perms \
	./scripts/check_printcap \
	./scripts/check_rhosts \
	./scripts/check_signatures \
	./scripts/check_system \
	./scripts/crack_run \
	./scripts/find_files \
	./util/buildbins \
	$(PLATFORM_SCRIPTS)

PLATFORM_SCRIPTS=./systems/NeXT/check \
	./systems/SunOS/4/check \
	./systems/SunOS/5/check

MISCFILES=./initdefs \
	./check.tbl \
	./syslist \
	./tigerrc \
	./cronrc \
	./util/difflogs \
	./util/flogit \
	./util/genmsgidx \
	./util/getfs-amd \
	./util/getfs-automount \
	./util/getfs-nfs \
	./util/getfs-std \
	./util/gethostinfo \
	./util/getnetgroup \
	./util/logit \
	./util/setsh \
	./util/sgrep

MISCDIRS=./c \
	./bin \
	./doc \
	./man \
	./scripts/sub \
	./systems

install:
	if [ ! -d $(TIGERHOME) ]; then \
	  mkdir -p $(TIGERHOME); \
	  chmod 755 $(TIGERHOME); \
        fi; \
	if [ ! -d $(TIGERHOME)/scripts ]; then \
	  mkdir $(TIGERHOME)/scripts; \
	  chmod 755 $(TIGERHOME)/scripts; \
	fi; \
	if [ ! -d $(TIGERHOME)/util ]; then \
	  mkdir $(TIGERHOME)/util; \
	  chmod 755 $(TIGERHOME)/util; \
	fi; \
        for dir in $(MISCDIRS); do \
	  tar cf - $$dir | (cd $(TIGERHOME); tar xpf -); \
	done; \
	for file in $(MISCFILES); do \
	  cp -p $$file $(TIGERHOME)/$$file; \
	done; \
	for file in $(SCRIPTS); do \
	  sed -e 's%^TigerInstallDir=.*$$%TigerInstallDir="'$(TIGERHOME)'"%' \
	      $$file > $(TIGERHOME)/$$file;\
	  chmod 755 $(TIGERHOME)/$$file; \
	done; \
	sed -e 's%^TigerLogDir=.*$$%TigerLogDir="'$(TIGERLOGS)'"%' \
	    -e 's%^TigerWorkDir=.*$$%TigerWorkDir="'$(TIGERWORK)'"%' \
	    -e 's%^TigerBinDir=.*$$%TigerBinDir="'$(TIGERBIN)'"%' \
	    ./config > $(TIGERHOME)/config; \
	chmod 644 $(TIGERHOME)/config;
	(cd $(TIGERHOME); ./util/genmsgidx doc/*.txt)



