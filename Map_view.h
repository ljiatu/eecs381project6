#include "Visual_view.h"

class Map_view : public Visual_view {
public:
	// default constructor sets the default size, scale, and origin
	Map_view(); 
	
	// prints out the current map
	void draw() override;
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_);

    // Set the origin as specified by the user.
    void set_origin(Point origin_)
    {origin = origin_;}
	
	// set the parameters to the default values
	void set_defaults();

private:
    int size;
    double scale;
    Point origin;
};
