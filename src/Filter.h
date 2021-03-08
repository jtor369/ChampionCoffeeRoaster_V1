

class DigitalIntegrator
{
    public:

        DigitalIntegrator(){
            b0 = 1.0;
            a1 = 1.0;
        }

        void SetGain(float gain){
            this->b0 = gain;
        }

        float GetGain(){
            return this->b0;
        }

        void CorrectY0(float y_0){
            y0 = y_0;
        }

        float ProcessSample(float x){
            x0 = x;
            y0 = x0 * b0 + zN;
            return y0;
        }

        void ProcessDelayLine(){
            x2 = x1;
            x1 = x0;
            y2 = y1;
            y1 = y0;
            zN = x1 * b1 + x2 * b2 + y1 * a1 + y2 * a2;
        }

    private:
        float a1 = 1.0, b0 = 1.0;
        float b1 = 0.0, b2 = 0.0;
        float a2 = 0.0;
     public:
        float x0,x1,x2;
        float y0,y1,y2;

        float zN = 0.0;

};

