/**********************************************************
Option Selection Display
options.h
Allows use of custom menu GUIs.
**********************************************************/

#include "display.h"

static const int maxoptions = 6; //maximum options count

static int startline = 7 - maxoptions; //line of origin for options
static string options[maxoptions][maxoptions]; //[option index][choice index]
static int optionscount = 0; //count of options (0-6)
static int choicecount[maxoptions]; //count of choices given to user

int    options_get[maxoptions];             //indices of selected choices (PUBLIC!)
char*  options_getname[maxoptions];         //names of each option (PUBLIC!)
char* options_getselectedname(int option)  //get name of selected option (PUBLIC!)
{
	if ((option < 0) || (option > maxoptions - 1)) { return ""; }
	return options[option][options_get[option]];
}

void options_reset()
{
	for (int i=0;	i<maxoptions; i++)
	{
		for (int j=0; j<maxoptions; j++)
		{
			options[i][j] = "";
		}
		choicecount[i] = 0;
		options_getname[i] = "";
		options_get[i] = 0;
	}
	 optionscount = 0;
}

void options_create(int option, char* name)
{
  if (option > maxoptions - 1) { return; }
  if (option < 0) { return; }
  options_getname[option] = name;
  optionscount++;
  return;
}

//BE SURE TO START AT OPTION ZERO... OR EVERYTHING DIES
void options_add(int option, char* choice)
{
	if (option > maxoptions - 1) { return; }
	if (option < 0) { return; }
	if (choicecount[option] >= maxoptions) { return; }

	int c = choicecount[option];
	options[option][c] = choice;
  choicecount[option]++;
}

void options_invert(int line)
{
  int max = display_y(line+startline+1)+1;
  int min = display_y(line+startline);
  for(int i=max; i<=min; i++)
  {
    nxtInvertLine(0, i, 99, i);
  }
}

//old and new are OPTIONS (starting from zero)
void options_select(int old, int new)
{
  if (old != -1) { options_invert(old); }
  options_invert(new);
}

void options_redisplay(int option, int choice)
{
	nxtEraseLine(0, display_y(option+startline), 99, display_y(option+startline+1)-1);
	nxtDisplayTextLine(option+startline, options_getname[option]);
	string c = options[option][choice];
  nxtDisplayStringAt(display_xright(strlen(c)), display_y(option+startline), c);
	options_select(-1, option);
}

/**
 * Displays options immediately to the screen
 * @param title of the window.  If the title is "", then no title will display.
 */
void options_display(char* title, char* confirmation)
{
  //Clear stuff
  diagnosticsOff();
  eraseDisplay();

  if (title == "") { startline = 0; }
  if (startline > 2) { startline = 2; }

  //Display caption and title
  nxtDisplayCenteredTextLine(0, title);
  nxtDisplayCenteredTextLine(7, confirmation);
  if (optionscount > maxoptions) { optionscount = maxoptions; }
  if (optionscount < 1) { return; }

  //Loop display default options
  for (int i=0; i<optionscount; i++)
  {
    nxtDisplayTextLine(i+startline, options_getname[i]);
    string d;
    StringFromChars(d, options[i][0]);
    nxtDisplayStringAt(display_xright(strlen(d)), display_y(i+startline), d);
  }

  //Display initial dot
  const int enter = maxoptions - startline + 1;
  options_select(-1, enter);
  int selected = enter;
  int selchoices[maxoptions];
  for (int i=0; i<maxoptions; i++) { selchoices[i]=0; }

  //Wait for user input
  while (true)
  {
    //ENTER button - rotate the item cycle
    if (nNxtButtonPressed == 3)
    {
        // If we're selecting the "OK" button
        if (selected == enter) {
          for (int i=0; i<maxoptions; i++)
          {
            options_get[i] = selchoices[i];
          }
          eraseDisplay();
          return;
        }
        // If the optioncount available is greater than one, rotate through
        if (choicecount[selected] > 1)
        {
        	selchoices[selected]++;
        	if (selchoices[selected] >= choicecount[selected])
        	{ selchoices[selected] = 0; }

        	//Display the rotated choice
        	options_redisplay(selected, selchoices[selected]);
      	}
      	wait10Msec(50);
    }
    //LEFT arrow - move up an item
    if (nNxtButtonPressed == 2)
    {
        if (selected == 0) {
          options_select(selected, enter);
          selected = enter;
        }
        else if (selected == enter) //if we're on the OK button
        {
          options_select(selected, optionscount - 1);
          selected = optionscount - 1;
        }
        else {
          options_select(selected, selected - 1);
          selected--;
        }
        wait10Msec(25);
    }
    //RIGHT arrow - move down an item
    if (nNxtButtonPressed == 1)
    {
    		if (selected == enter)
    		{
    			options_select(selected, 0);
          selected = 0;
    		}
        else if (selected == optionscount - 1)
        {
          //if we are on the last item, select the "OK" button
          options_select(selected, enter);
          selected = enter;
        }
        else {
	        options_select(selected, selected + 1);
	        selected++;
        }
        wait10Msec(25);
    }
    wait1Msec(10);
  }
}
