#include "Seeed_MCP9600_interface.h"


err_t sensor_basic_config(MCP9600 *_sensor)
{
    err_t ret=NO_ERROR;
    MCP9600 Q = *_sensor;
    CHECK_RESULT(ret,Q.set_filt_coefficients(FILT_MID));
    CHECK_RESULT(ret,Q.set_cold_junc_resolution(COLD_JUNC_RESOLUTION_0_25));
    CHECK_RESULT(ret,Q.set_ADC_meas_resolution(ADC_14BIT_RESOLUTION));
    CHECK_RESULT(ret,Q.set_burst_mode_samp(BURST_32_SAMPLE));
    CHECK_RESULT(ret,Q.set_sensor_mode(NORMAL_OPERATION));
    return ret;
}


err_t get_temperature(MCP9600 *_sensor, float *value)
{
    err_t ret=NO_ERROR;
    float hot_junc=0;
    float junc_delta=0;
    float cold_junc=0;
    MCP9600 Q = *_sensor;
    CHECK_RESULT(ret,Q.read_hot_junc(&hot_junc));
    CHECK_RESULT(ret,Q.read_junc_temp_delta(&junc_delta));
    
    CHECK_RESULT(ret,Q.read_cold_junc(&cold_junc));
    /*
    SERIAL.print("hot junc=");
    SERIAL.println(hot_junc);
    SERIAL.print("junc_delta=");
    SERIAL.println(junc_delta);
    SERIAL.print("cold_junc=");
    SERIAL.println(cold_junc);
    */
    *value=hot_junc;

    return ret;
}