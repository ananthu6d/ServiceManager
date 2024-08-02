#Application Version
APPVERSION="0.0.10"
#Application Release Date
APPRELDATE="31 July 2024"
CC= g++ -std=c++11 -w

#AppName
APPNAME=ServiceManager

#Setup
SETUP=/opt/Ananthu/Setups/ServiceManager

#Assign -g option to this vairable if Debug info is Required
DEBUG=-g -Wall 

DB_UTIL = /opt/VoiceProtocolTeam/GSP/Codes/General/DBUtilities/DBUtilitiesMYSQL/DBUtilitiesMYSQL_V3.0.3

REDIS=/opt/VoiceProtocolTeam/GSP/Codes/General/RedisInterface/RedisInterface_v1.0.0
#Base Directory for the GSP folder
core=/data/6d

#Variable for Header Includes
INCLUDES= -I$(core)/modules/v1.2/framework/include/  -I$(DB_UTIL)/include/ -I$(core)/STSCommonUtils/Includes/ -I/opt/VoiceProtocolTeam/GSP/Codes/ServiceManager/smi-protobuf-v1.0.0/ -I/usr/include/mysql -I$(DB_UTIL)/include/  -I../include/ -I/usr/local/include/sw/redis++/ -I$(REDIS)/include/ 

#Compiler Options for the source files
CFLAGS = -c -Wall -w '-DAPPVERSION=$(APPVERSION)' '-DAPPRELDATE=$(APPRELDATE)'  -D__LINUX__ -fpermissive -fexceptions

#Source Files(cpp) available in the folder
SRCS=$(wildcard ../src/*.cpp)

#OBject Files to be generated for the source files
OBJS=$(SRCS:%.cpp=%.o)
#External Objects to be linked with this
EXTOBJS= /usr/lib64/mysql/libmysqlclient.so.21.1.21 $(DB_UTIL)/build/libMySql.a -L$(core)/modules/v1.2/framework/lib/ -l6dEventLog -l6dNetwork -l6dFramework -l6dUtils -L/lib64 -lprotoc -lprotobuf /opt/VoiceProtocolTeam/GSP/Codes/ServiceManager/smi-protobuf-v1.0.0/libg6dServiceManagerInterface.a $(REDIS)/build/RedisInterface.a $(core)/STSCommonUtils/lib/libSTSCommonUtil_lnx_V2.5.a
#Archives to be linked with this
LIBS = $(GRPC) -L/usr/local/lib -lpthread -ldl -lcurl -rdynamic -ljansson -L/usr/local/lib/ -lhiredis -L/usr/local/lib64/ -lredis++ 

$(APPNAME) : $(OBJS)
	$(CC) $(DEBUG) $(OBJS) $(EXTOBJS) $(LIBS) -lz -o $(APPNAME).exe 
	rm -f ../src/*.o
	cp $(APPNAME).exe $(SETUP)
.cpp.o:
	$(CC) -c -o $*.o $(INCLUDES) $(CFLAGS) $*.cpp
clean :
	rm -f *.exe
	rm -f ../src/*.o
	rm -f $(SETUP)/$(APPNAME).exe
