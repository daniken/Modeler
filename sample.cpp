// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include <math.h>
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"


static GLfloat myLightPosition[] = { 0, 0, -3, 0 };

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:

	double time = 0.0;
	float blinkingEyeColor = 0.0f;


    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// variable that controls the color of the model
	double regColor = VAL(COLOR);

	// perametrization variable to be incremented at each draw() call
	if (ModelerApplication::Instance()->ModelerApplication::m_animating) time++;
	else time = 0;

	glLightfv(GL_LIGHT2, GL_POSITION, myLightPosition);

	// draw the floor
	//setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(246 / 256, 255 / 255, 123/255 );	// yellow color
	glPushMatrix();
	glTranslated(-5,-1.2,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	if (VAL(HIERARCHY) >= 0) {
		setAmbientColor(0.1f, 0.1f, 0.1f);
		setDiffuseColor(regColor, regColor, regColor);
		glPushMatrix();
		glTranslated(VAL(TURN_RADIUS) * (ModelerApplication::Instance()->ModelerApplication::m_animating == false) ? 0 : 1, 0, 0);
		glRotated(-time * VAL(SPEED) * 2, 0, 1, 0);
		glTranslated(VAL(XPOS) + VAL(TURN_RADIUS) * ((ModelerApplication::Instance()->ModelerApplication::m_animating == false) ? 0 : 1), VAL(YPOS), VAL(ZPOS));

		double bodyHeight = 3;
		double headRadius = VAL(HEADSIZE);
		double shoulderSize = 0.5;


		// Torso
		setAmbientColor(VAL(AMBIENTR), VAL(AMBIENTG), VAL(AMBIENTB));
		glPushMatrix();

		glRotated(-90 + VAL(TILT), 1, 0, 0);
		glTranslated(0, 0, VAL(HEIGHT) + 0.05*sin(time * 50 * 3.14 / 180));
		drawCylinder(bodyHeight, 1, 1);


		if (VAL(HIERARCHY) >= 1) {
			// draw torso details

			if (VAL(DETAILS) >= 1) {
				// upper slot
				setDiffuseColor(1 - regColor, 0.0f, regColor);
				glPushMatrix();
				glTranslated(-0.5, -1, 2.5);
				drawBox(0.9, 0.7, 0.2);
				glPopMatrix();
			}

			if (VAL(DETAILS) >= 2) {
				// lower slot
				glPushMatrix();
				glTranslated(-0.3, -1, 2.1);
				drawBox(0.5, 0.3, 0.2);
				glPopMatrix();
			}

			if (VAL(DETAILS) >= 3) {
				//side slot
				glPushMatrix();
				glTranslated(0.2, -1, 0.7);
				glRotated(21, 0, 0, 1);
				drawBox(0.5, 0.4, 1.1);
				glPopMatrix();
			}


			// head
			setDiffuseColor(regColor, regColor, regColor);
			glPushMatrix();
			glRotated(VAL(ROTATE), 0, 0, 1);
			glTranslated(0, 0, bodyHeight);
			drawSphere(headRadius);

			if (VAL(HIERARCHY) >= 2) {

				if (VAL(DETAILS) >= 1) {
					// head knob
					setDiffuseColor(1 - regColor, 0.0f, regColor);
					glPushMatrix();
					glRotated(65, 1, 1, 0);
					drawCylinder(headRadius*1.05 *(1 + abs(sin(time* 3.14 / 180))), 0.2, 0.2 * (1 - abs(sin(time* 3.14 / 180))));
					glPopMatrix();
				}


				if (VAL(DETAILS) >= 2) {
					blinkingEyeColor += 0.02f;
					if (blinkingEyeColor >= 1.0f) blinkingEyeColor = 0.0f;
					// red eye
					setDiffuseColor(regColor, blinkingEyeColor, 1 - regColor);
					glPushMatrix();
					glRotated(70, 1, 0, 0);
					glTranslated(0, 0, headRadius - 0.1);
					//glTranslated(0.1, -0.7, 0.2);
					drawSphere(0.2);
					glPopMatrix();
				}

				if (VAL(DETAILS) >= 3) {
					// draw antenna
					setDiffuseColor(1 - regColor, 0.0f, regColor);
					glPushMatrix();
					glTranslated(0, 0, headRadius);
					//glRotated(65, 1, 0, 0);
					drawCylinder(0.5, 0.05, 0.01);
					glPopMatrix();
				}
			}

			glPopMatrix(); // end of head 

			// draw shoulder 1
			setDiffuseColor(1 - regColor, 0.0f, regColor);
			glPushMatrix();
			glTranslated(0.7, -0.4, 2);
			drawBox(0.7, 0.8, 0.6);

			if (VAL(HIERARCHY) >= 3) {
				// draw arm 1
				setDiffuseColor(regColor, regColor, regColor);
				glPushMatrix();
				glRotated(-VAL(TILT), 1, 0, 0);
				glTranslated(0.4, 0.3, -2.7 - 0.05*sin(time * 50 * 3.14 / 180));
				drawBox(0.2, 0.2, 2.7);

				if (VAL(HIERARCHY) >= 4) {
					// draw hand 2
					setDiffuseColor(1 - regColor, 0.0f, regColor);
					glPushMatrix();
					glTranslated(0, -0.6, 0);
					drawBox(0.5, 1.4, 0.6);
					glPopMatrix();
				}

				glPopMatrix();	// end of arm 1
			}

			glPopMatrix(); // end of shoulder 1

			// draw shoulder 2
			setDiffuseColor(1 - regColor, 0.0f, regColor);
			glPushMatrix();
			glTranslated(-1.4, -0.4, 2);
			drawBox(0.7, 0.8, 0.6);

			if (VAL(HIERARCHY) >= 3) {
				// draw arm 2
				setDiffuseColor(regColor, regColor, regColor);
				glPushMatrix();
				glRotated(-VAL(TILT), 1, 0, 0);
				glTranslated(0.1, 0.3, -2.7 - 0.05*sin(time * 50 * 3.14 / 180));
				drawBox(0.2, 0.2, 2.7);

				if (VAL(HIERARCHY) >= 4) {
					// draw hand 2
					setDiffuseColor(1 - regColor, 0.0f, regColor);
					glPushMatrix();
					glTranslated(-0.3, -0.6, 0);
					drawBox(0.6, 1.4, 0.6);
					glPopMatrix();
				}

				glPopMatrix(); // end of arm 2
			}

			glPopMatrix(); // end of shoulder 2



			// draw  belt
			setDiffuseColor(1 - regColor, 0.0f, regColor);
			glPushMatrix();
			glTranslated(0, 0, -0.4);
			drawCylinder(0.4, 0.7, 1.0);

			if (VAL(HIERARCHY) >= 3) {
				// draw  leg
				setDiffuseColor(regColor, regColor, regColor);
				glPushMatrix();
				glTranslated(-0.2, -0.4, -0.5 - VAL(HEIGHT) - 0.05*sin(time * 50 * 3.14 / 180));
				drawBox(0.3, 0.8, 0.7 + VAL(HEIGHT) + 0.05*sin(time * 50 * 3.14 / 180));

				if (VAL(HIERARCHY) >= 4) {
					// draw  feet
					setDiffuseColor(1 - regColor, 0.0f, regColor);
					glPushMatrix();
					glRotated(-VAL(TILT), 1, 0, 0);
					glTranslated(-0.5, -0.3, -0.4);
					drawBox(1.2, 1.2, 0.6);
					glPopMatrix();
				}

				glPopMatrix(); // end of leg
			}

			glPopMatrix(); // end of belt
		}

		glPopMatrix(); // end of torso

		glPopMatrix(); // end of transformation
	}
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -15, 15, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 0, 5, 0.1f, 0);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[TILT] = ModelerControl("Tilt", -45, 37, 1, 0);
	controls[FLAP] = ModelerControl("Flap", 0, 90, 0.1, 0);
	controls[TURN_RADIUS] = ModelerControl("Turn radius", 0, 15, 0.1, 0);
	controls[SPEED] = ModelerControl("Turn speed", 0, 5, 1, 0);
	controls[HEADSIZE] = ModelerControl("Head size", 0.9, 10, 0.1, 0.9);
	controls[HIERARCHY] = ModelerControl("Hierarchy", 0, 4, -1, 4);
	controls[DETAILS] = ModelerControl("Detail level", 0, 3, 1, 3);
	controls[COLOR] = ModelerControl("Color", 0, 1, 0.01, 1);
	controls[AMBIENTR] = ModelerControl("Ambinet red", 0, 1, 0.01, 0);
	controls[AMBIENTG] = ModelerControl("Ambinet green", 0, 1, 0.01, 0);
	controls[AMBIENTB] = ModelerControl("Ambinet blue", 0, 1, 0.01, 0);



    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
