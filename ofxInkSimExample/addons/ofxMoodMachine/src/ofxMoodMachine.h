#pragma once

class ofxMoodMachine : public ofColor
{
	
public:
	
	ofxMoodMachine()
	: mood(true)
	, speed(1.f)
	{
		time_duration.r = 1000 * 0.001f;
		time_duration.g = 900 * 0.001f;
		time_duration.b = 1300 * 0.001f;
		
		color_from.r = ofRandom(255.f);
		color_from.g = ofRandom(255.f);
		color_from.b = ofRandom(255.f);
		
		color_to.r = ofRandom(255.f);
		color_to.g = ofRandom(255.f);
		color_to.b = ofRandom(255.f);
		
		ofAddListener(ofEvents().update, this, &ofxMoodMachine::update);
	}
	
	~ofxMoodMachine()
	{
		ofRemoveListener(ofEvents().update, this, &ofxMoodMachine::update);
	}
	
	inline void start() { mood = true; }
	inline void stop() { mood = false; }
	inline void toggle() { mood ^= true; }
	inline bool isMoody() const { return mood; }
	
	inline void setSpeed(float new_speed) { speed = new_speed; } // unofficial setter
	
private:
	
	typedef struct { float r = 0.f; float g = 0.f; float b = 0.f; } rgb_t;
	
	rgb_t time_elapsed;
	rgb_t time_duration;
	rgb_t color_from;
	rgb_t color_to;
	
	bool mood;
	float speed; // unofficial attribute
	
	void update(ofEventArgs &e)
	{
		if (isMoody() == false)
			return;
		
		const float t = ofGetLastFrameTime() * speed;
		
		time_elapsed.r += t;
		time_elapsed.g += t;
		time_elapsed.b += t;
		
		if (time_elapsed.r > time_duration.r)
		{
			color_from.r = color_to.r;
			color_to.r = ofRandom(255.f);
			time_elapsed.r = 0.f;
		}
		
		if (time_elapsed.g > time_duration.g)
		{
			color_from.g = color_to.g;
			color_to.g = ofRandom(255.f);
			time_elapsed.g = 0.f;
		}
		
		if (time_elapsed.b > time_duration.b)
		{
			color_from.b = color_to.b;
			color_to.b = ofRandom(255.f);
			time_elapsed.b = 0.f;
		}
		
		r = color_from.r + (color_to.r - color_from.r) * (time_elapsed.r / time_duration.r);
		g = color_from.g + (color_to.g - color_from.g) * (time_elapsed.g / time_duration.g);
		b = color_from.b + (color_to.b - color_from.b) * (time_elapsed.b / time_duration.b);
	}
};