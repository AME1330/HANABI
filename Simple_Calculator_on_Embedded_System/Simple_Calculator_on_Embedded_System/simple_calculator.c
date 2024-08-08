#include <stdio.h>
#include <math.h>
#include <string.h>


int Key_value = 0;                    //Each key should be for an actual number, that's the Key_value.
char Key[17] = "0123456789.+-x/=";    //Each Key_value is corresponding with a character in Key[17] as the index.

int Formula_index = 0;                
int last_Formula_index;               //To find the last operation.
char Formula[200];                    //Storing calculation which has been typed in.
int abstract_Formula[200];            //Storing every Key_value which has been typed in.

double items[15][5][6];               //This three-dimensional arrays is storing every numbers Participating in the calculation.
// in calculator data container array items:
//items[order][factor][0] is the magnification of number, its default value is 1.
//items[order][factor][1] is the power of number, its default value is 1. 
//items[order][factor][2] is the Integer part of number, its default value is 0.
//items[order][factor][3] is The decimal part of number, its default value is 0.  
//items[order][factor][4] is The Convert weights of number, its default value is 1.
//items[order][factor][5] indicates whether it exists or not, its default value is 0.
//for every number:  "magnification*£¨Integer part+£¨decimal part/Convert weights£©£©^power" is the real value of it.
//The order of calculations is determined by the index.


int order, factor, Fill_position;     //Index of the three-dimensional arrays items.
int factor_flag = 0;                  //Will be used if a multiplication sign were deleted.

double whight = 1;                    //Expression order of magnitude.
double temp = 0;
double sum = 0;




void calculator_init()
{
	order = 0;
	factor = 0;
	Fill_position = 2;
	Formula_index = 0;
	// Turn these indexes back to default values.


	for (int i = 0; i < 200; i++)
	{
		Formula[i] = NULL;
		abstract_Formula[i] = NULL;
	}
	// Clear the calculation.

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			items[i][j][0] = 1;
			items[i][j][1] = 1;
			items[i][j][2] = 0;
			items[i][j][3] = 0;
			items[i][j][4] = 1;
			items[i][j][5] = 0;
		}
	}
	//Add default values to the calculator data container array items.
}





//Multiply existing data in groups.
//The results of each group of multiplication are then added together to obtain the result.
double Compute()
{
	sum = 0;
	double factors[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 15; i++)
	{

		for (int j = 0; j < 5; j++)    //Determine whether there is a numeric value in the group.
		{
			if (items[i][j][5])
			{
				factors[i] = 1;
			}
		}
		for (int j = 0; j < 5; j++)
		{
			if (items[i][j][5])     //Determine whether there is a numeric value in the place.
			{
				temp = items[i][j][0] * (items[i][j][2] + (items[i][j][3] / items[i][j][4]));
				temp = pow(temp, items[i][j][1]);
				factors[i] *= temp;
			}
		}
		sum += factors[i];
	}
	return sum;
}


//Delete or undo the operation based on the record in char Formula and abstract_Formula.
void delete()
{
	{
		switch (abstract_Formula[Formula_index])
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			if (abstract_Formula[last_Formula_index] > 9)
			{
				items[order][factor][5] = 0;
			}
			items[order][factor][Fill_position] -= abstract_Formula[Formula_index];
			items[order][factor][Fill_position] /= 10;
			if (Fill_position == 3)
			{
				items[order][factor][4] /= 10;
				whight = items[order][factor][4];
			}
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			switch (abstract_Formula[Formula_index])
			{
			case 10:
				Fill_position = 2;
				break;
			case 11:
			case 12:
				if (abstract_Formula[last_Formula_index] == 13 || abstract_Formula[last_Formula_index] == 14)
				{
					items[order][factor][0] = 1;
				}
				else
				{
					items[order][factor][5] = 0;
					order--;
				last_factor_finding:
					if (!factor_flag)
					{
						for (int i = 4; i >= 0; i--)
						{
							if (items[order][factor][5])
							{
								factor = i;
								factor_flag = 1;
								goto last_factor_finding;
							}
						}
					}
					factor_flag = 0;
				}
				if (items[order][factor][4] > 1)
				{
					Fill_position = 3;
					whight = items[order][factor][4];
				}
				else
				{
					Fill_position = 2;
				}
				break;
			case 13:
			case 14:
				items[order][factor][5] = 0;
				factor--;
				if (items[order][factor][4] > 1)
				{
					Fill_position = 3;
					whight = items[order][factor][4];
				}
				else
				{
					Fill_position = 2;
				}
				break;
			}
		}
	}
}


//Record the order of input and formula to be displayed.
void Formula_dispaly()
{
	if (Key_value == 16)
	{
		Formula_index--;
		last_Formula_index = Formula_index - 1;
		delete();
		Formula[Formula_index] = NULL;
		abstract_Formula[Formula_index] = NULL;
	}
	else
	{
		last_Formula_index = Formula_index;
		Formula[Formula_index] = Key[Key_value];
		abstract_Formula[Formula_index] = Key_value;
		Formula_index++;
	}
	//The display of the calculation is updated here, "Formula" is the calculation to dispaly.
	printf("Calculation: %s\n", Formula);
}


//calculator() Responds to the main program input value..
//Populate the array of items with data.
//Fill the input data in place by moving the indexes "order" and "factor".
void calculator()
{
	switch (Key_value)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		items[order][factor][5] = 1;
		items[order][factor][Fill_position] *= 10;
		items[order][factor][Fill_position] += Key_value;
		whight *= 10;
		if (Fill_position == 3)
		{
			items[order][factor][4] = whight;
		}
		Formula_dispaly();
		break;
	case 10:
		if (Formula[last_Formula_index] == '.')
		{
		}
		else
		{
			Fill_position = 3;
			whight = 1;
		}
		Formula_dispaly();
		break;
	case 11:
	case 12:
		if (Formula[last_Formula_index] == '+' || Formula[last_Formula_index] == '-')
		{
		}
		else if (Formula[last_Formula_index] == 'x' || Formula[last_Formula_index] == '/')
		{
			if (Key_value == 11)
			{
				items[order][factor][0] = 1;
			}
			else
			{
				items[order][factor][0] = -1;
				Formula_dispaly();
			}
		}
		else
		{
			order++;
			factor = 0;
			Fill_position = 2;
			whight = 1;
			if (Key_value == 11)
			{
				items[order][factor][0] = 1;
			}
			else
			{
				items[order][factor][0] = -1;
			}
			Formula_dispaly();
		}
		break;
	case 13:
	case 14:
		if (Formula[last_Formula_index] == '+' || Formula[last_Formula_index] == '-' || Formula[last_Formula_index] == 'x' || Formula[last_Formula_index] == '/')
		{
		}
		else
		{
			factor++;
			Fill_position = 2;
			whight = 1;
			if (Key_value == 13)
			{
				items[order][factor][1] = 1;
			}
			else
			{
				items[order][factor][1] = -1;
			}
			Formula_dispaly();
		}
		break;
	case 15:
		Formula_dispaly();
		sum = Compute();
		calculator_init();

		// The result of the calculation is computed here, "sum" is the result of calculator.
		printf("Result:%lf\n", sum);
		break;
	case 16:
		Formula_dispaly();
		break;
	default:
		break;
	}
}

//Query buttons.
int main()
{
	calculator_init();
	while (1)
	{
		scanf("%d", &Key_value);
		calculator();
	}
}
