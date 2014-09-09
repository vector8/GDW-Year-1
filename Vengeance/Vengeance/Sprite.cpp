#include "Sprite.h"

/* THE SPRITE CLASS DEFINITION */
Sprite::Sprite(std::string spriteSheetFilename)
{
	this->filename = spriteSheetFilename;
	this->loadSpriteSheet(spriteSheetFilename.c_str());
	currentAnimation = 0;
	layerID = 0;
	theta = 0;
	centerX = centerY = 0;
	flipHoriz = false;
}

Sprite::~Sprite(void)
{
}

void Sprite::initialize(int numAnimations, int frameWidth, int frameHeight, float posX, float posY, float layerID)
{
	this->setNumberOfAnimations(numAnimations);
	this->setSpriteFrameSize(frameWidth, frameHeight);
	this->setPosition(posX, posY);
	this->setLayerID(layerID);
}

void Sprite::loadSpriteSheet(const char *filename)
{
	sheet.textureID = ilutGLLoadImage((char*)filename);
	sheet.width = ilGetInteger(IL_IMAGE_WIDTH);
	sheet.height = ilGetInteger(IL_IMAGE_HEIGHT);
}

void Sprite::setSpriteFrameSize(int width, int height)
{
	sz.width = width;
	sz.height = height;
	sz.normalizedWidth = (float)width/(float)sheet.width;
	sz.normalizedHeight = (float)height/(float)sheet.height;

	centerX = width / 2;
	centerY = height / 2;
}

void Sprite::setNumberOfAnimations(int num)
{
	numberOfAnimations = num;
	animations.reserve(num);
	for(int i=0;i<num;i++)
	{
		SpriteAnimation *anim = new SpriteAnimation();
		animations.push_back(anim);
	}
}

/*
 * addSpriteAnimFrame()
 *  - this adds a sprite animation frame to the end of the particular animation list
 *  - (x,y) specify the pixel values of the bottom left corner of the frame
 *  - animationNumber specifies which animation (i.e. JUMP, RUN, etc)
 *    you could use enum's for easier coding 
 */
void Sprite::addSpriteAnimFrame(int animationNumber, int x, int y)
{
	float u,v;
	u = (float)x/sheet.width;
	v = (float)y/sheet.height;

	XY_Coordinate *xy = new XY_Coordinate(x,y, u,v);
	animations[animationNumber]->coords.push_back(xy);
}

void Sprite::addSpriteAnimRow(int animationNumber, float startX, float startY, float spacingX, float spacingY, int numFrames)
{
	float x,y;
	x = startX;
	y = startY;
	for(int i=0;i<numFrames;i++)
	{
		addSpriteAnimFrame(animationNumber, x,y);
		x += spacingX;
		y += spacingY;
	}
}

/*
 * draw()
 * - this draws the sprite texture at its current frame of its current animation
 * - it positions the sprite based on the transformation formula:
 *    v' = RotationMatrix*(v-center) + position
 * where position is the bottom left corner of the sprite
 * and v is each corner of the sprite rectangle, v' is the transformed corner
 * and RotationMatrix is defined by the sprite's theta value (counter clockwise)
 */
void Sprite::draw()
{
	/* draw the sprite */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();

	/* position the sprite into the particular pixel location */
	glTranslatef(positionX,positionY,0);

	// apply the rotation around the center of the sprite
	glTranslatef(centerX,centerY,0);
	if(flipHoriz)
		glRotatef(180, 0, 1, 0);
	glRotatef(theta, 0,0,1);
	glTranslatef(-centerX,-centerY,0);

	/* get the texture coordinate from the sprite so we know which frame to draw */
	SpriteAnimation *anim = animations[currentAnimation];
	int currentFrame = anim->currentFrame;
	float u,v;
	u = anim->coords[currentFrame]->u;
	v = anim->coords[currentFrame]->v;

	glColor3f(1,1,1);
		/* bind the appropriate texture frame */
		glBindTexture(GL_TEXTURE_2D,sheet.textureID);
		/* draw the image as a quad the size of the first loaded image */
		glBegin(GL_QUADS);
		    glTexCoord2f(u,v);
			glVertex3f(0,0,0);
			
			glTexCoord2f(u,v+sz.normalizedHeight);
			glVertex3f(0,sz.height,0);
			
			glTexCoord2f(u+sz.normalizedWidth,v+sz.normalizedHeight);
			glVertex3f(sz.width,sz.height,0);
			
			glTexCoord2f(u+sz.normalizedWidth,v);
			glVertex3f(sz.width,0,0);
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
}

// NO LONGER USED
/* update()
 * - this function could update things like physics and position
 * dt - change in time since last update (in milliseconds!)
 */
//void Sprite::update(float dt)
//{
//	positionX = positionX + dx*(dt/1000);
//	positionY = positionY + dy*(dt/1000);
//	if(positionX > 800) positionX = 0;
//}