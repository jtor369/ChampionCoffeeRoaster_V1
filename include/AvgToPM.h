#include <stdlib.h>

class AvgToPM
{
    public:

        AvgToPM(__UINT8_TYPE__ mean_size) : index(0), avg(0.0){
            this->avg_array = (bool*)calloc(mean_size,sizeof(bool));
            this->mean_size = mean_size;
        }

        bool GetSignal(float avg_target){
            bool ret = false;
            if (avg_target > 0.0){
                this->avg = this->calc_avg();
                if (this->avg <= avg_target){
                    ret = true;
                }
            }
            this->add_state(ret);
            return ret;
        }

    private:
        float calc_avg()
        {
            float res = 0.0;
            for (int i = 0; i < this->mean_size; i++){
                res += this->avg_array[i] ? 1.0 : 0.0;
            }
            res /= this->mean_size;

            return res;
        }

        void add_state(bool state){
            this->avg_array[this->index] = state;
            this->index++;
            if (this->index >= mean_size){
                this->index = 0;
            }
        }

        __UINT8_TYPE__ mean_size;
        __UINT8_TYPE__ index;
        bool *avg_array;
        float avg;

};

