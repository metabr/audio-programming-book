#include <cstdlib>

class irand_osc {
	public:
		irand_osc() : magnitude_(0),r1_(0.0),r2_(0.0),phase_(0.0),inv_srate_(0.0),inv_randmax_(1.0/RAND_MAX) {}
		void init(unsigned long srate) {
			inv_srate_ = 1.0 / srate;
			r1_ = std::rand() * inv_randmax_;
			r2_ = std::rand() * inv_randmax_;
			magnitude_ = r2_ - r1_;
			phase_ = 0.0;
		}
			
		double tick(double freq){
			double val;
	
			val= r1_ + phase_ * magnitude_;
			phase_ += freq * inv_srate_;
			if(phase_ >= 1.0){
				phase_ -= 1.0;
				r1_ = r2_;
				r2_ = std::rand() * inv_randmax_;
				magnitude_ = r2_ - r1_;
			}
			return val;
		}
	private:
        double magnitude_;
        double r1_,r2_;
        double phase_;
        double inv_srate_;
        const double inv_randmax_;
};


	
	