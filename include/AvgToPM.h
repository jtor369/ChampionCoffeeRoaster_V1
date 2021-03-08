#include <stdlib.h>

class AvgToPM
{
    public:

        AvgToPM(__UINT8_TYPE__ mean_size) : index(0), avg(0.0){
            this->avg_array = (bool*)calloc(mean_size,sizeof(bool));
            this->mean_size = mean_size;
        }

        bool GetSignal(float avg_target){
            if (this->avg != avg_target){
                GenerateArray(avg_target);
            }
            return next();
        }

    private:
        void GenerateArray(float avg_target){
            bool ret = false;
            float cavg = 0.0;
            for (int i = 0; i < mean_size; i++){
                            
            if (avg_target > 0.0){
                this->avg = cavg / (i+1);
                if (this->avg <= avg_target){
                    ret = true;
                }else
                {ret = false;}
                cavg += ret ? 1.0 : 0.0;
            }
            this->avg_array[i] = ret;
            //this->add_state(ret);
            }
            this->avg = avg_target;
            /*
            Serial.print("[ ");
            for (int i = 0; i < mean_size-1; i++){
                Serial.print(avg_array[i] ? "1, " : "0, ");

            }
            Serial.print(avg_array[mean_size-1] ? "1" : "0");
             Serial.println(" ];");
             */
       }

        float calc_avg()
        {
            float res = 0.0;
            for (int i = 0; i < this->mean_size; i++){
                res += this->avg_array[i] ? 1.0 : 0.0;
            }
            res /= this->mean_size;

            return res;
        }
        bool next(){
            bool ret = false;
            ret = avg_array[index++];
            if (index >= mean_size){
                index = 0;
            }
            return ret;
        }

/*
        void add_state(bool state){
            this->avg_array[this->index] = state;
            this->index++;
            if (this->index >= mean_size){
                this->index = 0;
            }
        }
        */

        __UINT8_TYPE__ mean_size;
        __UINT8_TYPE__ index;
        bool *avg_array;
        float avg;

};

