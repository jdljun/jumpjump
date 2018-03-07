#ifndef MAIN_H
#define MAIN_H

//Called for initiation
bool Init(void);

//Called to draw scene
void Display(void);

//Called on window resize
void Reshape(int w, int h);

//Called when a key is pressed
void Keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv);

#endif	//MAIN_H
