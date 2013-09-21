#include <ghack.h>
#include <gh_sdl.h>

using namespace std;

#define GH_MAP_WIDTH 100
#define GH_MAP_HEIGHT 100

#define GH_MAX_HEIGHT 100
#define GH_MAX_LEVEL 20

float gh_height_map[GH_MAP_WIDTH][GH_MAP_HEIGHT];

vector<int> gh_fractal_boxes[GH_MAX_LEVEL];

float gauss (const float m, const float s)
{
    static int use_last = 0;
    static float y2;
    float x1, x2, w, y1;

    if (use_last) {
	y1 = y2;
    } else {
	do { // ming don't have random
	    x1 = 2.0 * ((float)rand() / (float)RAND_MAX) - 1.0;
	    x2 = 2.0 * ((float)rand() / (float)RAND_MAX) - 1.0;
	    w = x1 * x1 + x2 * x2;
	} while (w >= 1.0);

	w = sqrt((-2.0 * log(w)) / w);
	y1 = x1 * w;
	y2 = x2 * w;
    }

    use_last = !use_last;

    return (m + y1 * s);
}

//
// Break a box into four more smaller boxes and store them at the next
// recurse level.
//
// (x1,y1)-----(x2,y1)-----(x3,y1)
//    |           |           |
//    |           |           |
// (x1,y2)-----(x2,y2)-----(x3,y2)
//    |           |           |
//    |           |           |
// (x1,y3)-----(x2,y3)-----(x3,y3)
//
//
void gh_fractal_make_boxes (int x1,
			    int y1,
			    int x3,
			    int y3,
			    int level)
{
    gh_fractal_boxes[level].push_back(x1);
    gh_fractal_boxes[level].push_back(y1);
    gh_fractal_boxes[level].push_back(x3);
    gh_fractal_boxes[level].push_back(y3);

    if (x3 - x1 == 1) {
	return;
    }

    level++;

    if (level >= GH_MAX_LEVEL) {
	return;
    }

    int x2 = ((x3 - x1) / 2) + x1;
    int y2 = ((y3 - y1) / 2) + y1;

    gh_fractal_make_boxes(x1, y1, x2, y2, level);
    gh_fractal_make_boxes(x2, y1, x3, y2, level);
    gh_fractal_make_boxes(x1, y2, x2, y3, level);
    gh_fractal_make_boxes(x2, y2, x3, y3, level);
}

//
// Walk the current box (and then the next level below) and assign heights
// to the corners. We avoid recursion here and do each level one at a time
// to avoid any boxing of the landscape.
//
// (x1,y1)-----(x2,y1)-----(x3,y1)
//    |           |           |
//    |           |           |
// (x1,y2)-----(x2,y2)-----(x3,y2)
//    |           |           |
//    |           |           |
// (x1,y3)-----(x2,y3)-----(x3,y3)
//
//

void gh_fractal_make_map (float stdev, float stdev_shrink_factor)
{
    vector<int>::iterator s;

    for (int l = 0; l < GH_MAX_LEVEL; l++) {
	for (s = gh_fractal_boxes[l].begin(); s != gh_fractal_boxes[l].end(); ) {
	    int x1 = *s++;
	    int y1 = *s++;
	    int x3 = *s++;
	    int y3 = *s++;

	    int x2 = ((x3 - x1) / 2) + x1;
	    int y2 = ((y3 - y1) / 2) + y1;

	    gh_height_map[x2][y1] =
		gauss(((gh_height_map[x1][y1] + gh_height_map[x3][y1]) / 2.0),
		      stdev);
	    gh_height_map[x1][y2] =
		gauss(((gh_height_map[x1][y1] + gh_height_map[x1][y3]) / 2.0),
		      stdev);
	    gh_height_map[x2][y3] =
		gauss(((gh_height_map[x1][y3] + gh_height_map[x3][y3]) / 2.0),
		      stdev);
	    gh_height_map[x3][y2] =
		gauss(((gh_height_map[x3][y1] + gh_height_map[x3][y3]) / 2.0),
		      stdev);
	    gh_height_map[x2][y2] =
		gauss(((gh_height_map[x1][y1] + gh_height_map[x3][y1] +
		        gh_height_map[x1][y3] + gh_height_map[x3][y3]) / 4.0),
		      stdev);
	}

	stdev *= stdev_shrink_factor;

	if (!(int)stdev) {
	    stdev = 1.0;
	}
    }
}

void gh_make_landscape (int seed, float stdev, float stdev_shrink_factor)
{
    memset(gh_height_map, 0, sizeof(gh_height_map));

    srand(seed);

    gh_height_map[0][0] 				=
	rand() % GH_MAX_HEIGHT;
    gh_height_map[GH_MAP_WIDTH - 1][0] 			=
	rand() % GH_MAX_HEIGHT;
    gh_height_map[0][GH_MAP_HEIGHT - 1] 		=
	rand() % GH_MAX_HEIGHT;
    gh_height_map[GH_MAP_WIDTH - 1][GH_MAP_HEIGHT - 1] 	=
	rand() % GH_MAX_HEIGHT;

    gh_fractal_make_map(stdev, stdev_shrink_factor);

    for (int y = 0; y < GH_MAP_HEIGHT; y++) {
	for (int x = 0; x < GH_MAP_WIDTH; x++) {
	    if (gh_height_map[x][y] > 120) {
		cout << "^";
	    } else if (gh_height_map[x][y] > 100) {
		cout << "&";
	    } else if (gh_height_map[x][y] > 90) {
		cout << "=";
	    } else if (gh_height_map[x][y] > 80) {
		cout << ":";
	    } else if (gh_height_map[x][y] > 70) {
		cout << ".";
	    } else {
		cout << " ";
	    }
	}
	cout << endl;
    }
}

int main (int argc, char *argv[])
{
    int seed;
    float stdev;
    float stdev_shrink_factor;

    if (argc > 1) {
	seed = atoi(argv[1]);
    } else {
	seed = 1;
    }

    if (argc > 2) {
	stdev = atof(argv[2]);
    } else {
	stdev = 20;
    }

    if (argc > 3) {
	stdev_shrink_factor = atof(argv[3]);
    } else {
	stdev_shrink_factor = 0.8;
    }

    gh_fractal_make_boxes(0, 0, GH_MAP_WIDTH-1, GH_MAP_HEIGHT-1, 0);

    gh_make_landscape(seed, stdev, stdev_shrink_factor);

    return (0);
}
