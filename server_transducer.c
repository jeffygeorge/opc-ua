// connnected with transducer and reading values

#include <signal.h>
#include "open62541.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <modbus/modbus.h>

//serial number
static void addSerialNum(UA_Server *server)
{

		UA_VariableAttributes attr = UA_VariableAttributes_default;
		UA_Int32 myInteger ;

		 modbus_t *ctx;
		    uint16_t tab_reg[64];
		    int rc;
		    int i;

		    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
		    if (modbus_connect(ctx) == -1)
		    {
		        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		        modbus_free(ctx);
		        return ;
		    }

		    modbus_set_slave(ctx, 1);


		    rc = modbus_read_registers(ctx, 149, 1, tab_reg);

		        if (rc == -1)
		        {
		            fprintf(stderr, "read failed: %s\n", modbus_strerror(errno));
		            return ;
		        }

		        for (i=0; i < rc; i++)
		        {
		            printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
		        }

		        myInteger=tab_reg[0];
		        modbus_close(ctx);
		        modbus_free(ctx);





		UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
		attr.description = UA_LOCALIZEDTEXT("en-US","serial number");
		attr.displayName = UA_LOCALIZEDTEXT("en-US","serial number");
		attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
		attr.accessLevel = UA_ACCESSLEVELMASK_READ ;
		/* Adding to the information model */
		UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "serial number");
		UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "serial number");
		UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,parentReferenceNodeId, myIntegerName,UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),attr, NULL, NULL);
}

//software version
static void addSWVersion(UA_Server *server)
{

		UA_VariableAttributes attr = UA_VariableAttributes_default;
		UA_Int32 myInteger ;

		 modbus_t *ctx;
		    uint16_t tab_reg[64];
		    int rc;
		    int i;

		    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
		    if (modbus_connect(ctx) == -1)
		    {
		        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		        modbus_free(ctx);
		        return ;
		    }

		    modbus_set_slave(ctx, 1);
		    printf("reading... \n");

		    rc = modbus_read_registers(ctx, 31, 1, tab_reg);

		        if (rc == -1)
		        {
		            fprintf(stderr, "read failed: %s\n", modbus_strerror(errno));
		            return ;
		        }

		        for (i=0; i < rc; i++)
		        {
		            printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
		        }

		        myInteger=tab_reg[0];
		        modbus_close(ctx);
		        modbus_free(ctx);





		UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
		attr.description = UA_LOCALIZEDTEXT("en-US","software-version");
		attr.displayName = UA_LOCALIZEDTEXT("en-US","software-version");
		attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
		attr.accessLevel = UA_ACCESSLEVELMASK_READ ;
		/* Adding to the information model */
		UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "sw-version");
		UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "sw-version");
		UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,parentReferenceNodeId, myIntegerName,UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),attr, NULL, NULL);
}


static UA_StatusCode readCurrentTime(UA_Server *server,const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext,UA_Boolean sourceTimeStamp, const UA_NumericRange *range,UA_DataValue *dataValue)
{
	    UA_Int32 now;
	    modbus_t *ctx;
	    uint16_t tab_reg[64];
	    int rc;
	    int i;

	    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
	    if (modbus_connect(ctx) == -1) {
	        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
	        modbus_free(ctx);
	        return UA_STATUSCODE_BADOUTOFMEMORY;
	    }

	    modbus_set_slave(ctx, 1);
	    printf("reading... \n");
	// modbus_write_register(modbus_t *ctx, int addr, int value);
	    rc = modbus_read_registers(ctx, 10, 1, tab_reg);

	    if (rc == -1) {
	        fprintf(stderr, "read failed: %s\n", modbus_strerror(errno));
	        return UA_STATUSCODE_BADOUTOFMEMORY;
	    }

	    for (i=0; i < rc; i++) {
	        printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
	    }
	    now=tab_reg[0];
	    modbus_close(ctx);
	    modbus_free(ctx);



	UA_Variant_setScalarCopy(&dataValue->value, &now, &UA_TYPES[UA_TYPES_INT32]);
	dataValue->hasValue = true;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode writeCurrentTime(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data)
{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Changing the system time is not implemented");
		return UA_STATUSCODE_BADINTERNALERROR;
}
static void addCurrentTimeVoltageValueVariable(UA_Server *server)
{
		UA_VariableAttributes attr = UA_VariableAttributes_default;
		attr.displayName = UA_LOCALIZEDTEXT("en-US", "Voltage value ");
		attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
		UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Voltage value");
		UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Voltage value");
		UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
		UA_DataSource timeDataSource;
		timeDataSource.read = readCurrentTime;
		timeDataSource.write = writeCurrentTime;
		UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,parentReferenceNodeId, currentName,variableTypeNodeId, attr,timeDataSource, NULL, NULL);
}

UA_Boolean running = true;
static void stopHandler(int sign)
{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
		running = false;
}
int main(void)
{
		signal(SIGINT, stopHandler);
		signal(SIGTERM, stopHandler);

		UA_ServerConfig *config = UA_ServerConfig_new_default();
		UA_Server *server = UA_Server_new(config);

		addCurrentTimeVoltageValueVariable(server);
		addSerialNum(server);
		addSWVersion(server);

		UA_StatusCode retval = UA_Server_run(server, &running);
		UA_Server_delete(server);
		UA_ServerConfig_delete(config);
		return (int)retval;
}
