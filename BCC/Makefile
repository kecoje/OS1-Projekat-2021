SRC = ..\source\*.cpp 
INCLUDES = ..\header\*.h 
LIBS = ..\lib\*.lib 
INCLUDEPATH = C:\bc31\include;..\header;
LIBSPATH = C:\bc31\lib;..\lib;
TARGET = os1projekat.exe
$(TARGET):	$(SRC) $(INCLUDES) configuration.xmi
	bcc.exe -mh -v -e$(TARGET) -I$(INCLUDEPATH) -L$(LIBSPATH) $(SRC) $(LIBS)
all:	$(TARGET)
clean:
	-del *.obj
	-del $(TARGET)
	-del *.asm
configuration.xmi:	
	configuration.xmi