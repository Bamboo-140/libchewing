OS=`uname`

ifeq ("$(shell uname)", "Darwin")

	INST_PATH=/Library/OpenVanilla/0.7.1/Modules/
	OBJS=$(patsubst %, %.o, $(SOURCES))
	GOALS=$(IMID).dylib

$(GOALS): $(OBJS)
	$(CPP) -bundle $(LDFLAGS) $(LIBS) -o $@ $(OBJS)

install-goal:
	$(CP) $(GOALS) $(INST_PATH)


else


	INST_PATH=/usr/local/lib/openvanilla/
	OBJS=$(patsubst %, %.lo, $(SOURCES))
	GOALS=$(IMID).la

$(GOALS): $(OBJS)
	$(LIBTOOL) --tag=CXX --mode=link $(CPP) $(LDFLAGS) $(LIBS) -module -rpath $(INST_PATH) -avoid-version -o $(GOALS)  $(OBJS)

install-goal:
	$(MKDIR) $(INST_PATH)
	$(LIBTOOL) --mode=install install $(GOALS) $(INST_PATH)

endif
