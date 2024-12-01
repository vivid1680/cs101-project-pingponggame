#include <iostream>
using namespace std;

int main()
{
    int x = 2;
    int row1 = 12 * x;
    int col2 = (8 * x) + 1;

    for (int i = 1; i <= row1; i++)
    {
        for (int j = 1; j <= col2; j++)
        {
            if (i + j == x * 5 || i + j == (x * 16) + 1 
                || j - i == (x * 4) || i - j == (x * 7) + 1 || ((i > (row1 / 3)) 
                && (i < (2 * row1 / 3)) && (j == 1)) || ((i < (2 * row1 / 3)) 
                && (i > (row1 / 3)) && (j == col2)))
            {
                if (i % 2 == 0)
                {
                    cout << "- ";
                }
                else
                {
                    cout << "* ";
                }
            }
            else if (((i > 2) && (i < (row1 - 1)) && (j > 2) 
                && (j < (col2 - 1))) && (i + j == (6 * x) || i + j == (x * 15) + 1 
                || j - i == (x * 3) || i - j == (x * 6) + 1 || ((i > (row1 / 3)) 
                && (i < (2 * row1 / 3)) && (j == 3)) ||((i < (2 * row1 / 3)) 
                && (i > (row1 / 3)) && (j == (col2 - 2)))))
            {
                if (i % 2 == 0)
                {
                    cout << "* ";
                }
                else
                {
                    cout << "- ";
                }
            }
            else if ((i > (x * 3) + 1) && (i < (x * 8) + 1) 
                && (j > (x * 2)) && (j < (x * 7)))
            {
                if (i + j <= (x * 8) || i + j >= (x * 13) || j - i >= x || i - j >= (x * 4))
                {
                    cout << "* ";
                }
                else if (i + j == (x * 8) + 1 || j - i == (x - 1) || i - j == (x * 3) +1)
                {
                    cout << "1 ";
                }
                else if (i + j == (x * 9) || i == j || i - j == (x * 3))
                {
                    cout << "2 ";
                }
                else if (i + j == (x * 9) + 1 || j - i == -1 || i - j == (x * 2) + 1)
                {
                    if (j == (x * 6) || i == (x * 7) + 1)
                    {
                        cout << "  ";
                    }
                    else
                    {
                        cout << "3 ";
                    }
                }
                else if (i + j == (x * 10) || j - i == -x || i - j == (x * 2))
                {
                    if (j == (x * 5) + 1 || i == (x * 7))
                    {
                        cout << "  ";
                    }
                    else
                    {
                        cout << "4 ";
                    }
                }
                else if (i + j == (x * 10) + 1 || j - i == -x - 1 || i - j == x + 1)
                {
                    if (j == (x * 5) || i == (x * 7))
                    {
                        cout << "  ";
                    }
                    else
                    {
                        cout << "5 ";
                    }
                }
            }
            else
            {
                cout << "  ";
            }
        }
        cout << "\n";
    }
    return 0;
}
/* solving this task seperately into three parts, first the outer hexagon:
#include <iostream>
using namespace std;

int main() {
    int row, col;
    
    for (row = 1; row <= 9; row++) {
        for (col = 1; col <= 17; col++) {
            if (row + col == 10|| col-row==8) {
                if (row % 2 == 0) {
                    cout << " -";
                } else {
                    cout << " *";
                }
            } else {
                cout << "  "; // Spacing to maintain alignment
            }
        }
        cout << endl;
    }
    
    for(int row2=1;row2<=6;row2++){
    	for(int col2=1;col2<=17;col2++){
    		if(col2==1||col2==17){
    			if(row2%2==0){
				cout<<" *";}
    			else{
    				cout<<" -";
				}
			}else{
				cout<<"  ";
			}
		}
    	
    	
    	cout<<endl;
    	
	}
	for (int row3 = 9; row3 >= 1; row3--) {
        for (int col3 = 1; col3 <= 17; col3++) {
            if (row3 + col3 == 10|| col3-row3==8) {
                if (row3 % 2 == 0) {
                    cout << " *";
                } else {
                    cout << " -";
                }
            } else {
                cout << "  "; // Spacing to maintain alignment
            }
        }
        cout << endl;
    }
    

    return 0;
}


Then the inner hexagon:
#include <iostream>
using namespace std;

int main() {
    int row, col;
    
    for (row = 1; row <= 7; row++) {
        for (col = 1; col <= 13; col++) {
            if (row + col == 8|| col-row==6) {
                if (row % 2 == 0) {
                    cout << " *";
                } else {
                    cout << " -";
                }
            
            } else {
                cout << "  "; // Spacing to maintain alignment
            }
        }
        cout << endl;
    }
    
    
    
    for(int row2=1;row2<=6;row2++){
    	for(int col2=1;col2<=13;col2++){
    		if(col2==1||col2==13){
    			if(row2%2==0){
				cout<<" -";}
    			else{
    				cout<<" *";
				}
			}else{
				cout<<"  ";
			}
		}
    	
    	
    	cout<<endl;
    	
	}
	for (int row3 = 7; row3 >= 1; row3--) {
        for (int col3 = 1; col3 <= 13; col3++) {
            if (row3 + col3 == 8|| col3-row3==6) {
                if (row3 % 2 == 0) {
                    cout << " -";
                } else {
                    cout << " *";
                }
            } else {
                cout << "  "; // Spacing to maintain alignment
            }
        }
        cout << endl;
    }
    

    return 0;
}


Then the inner pattern:
#include <iostream>
using namespace std;

int main() {
    int n = 5; // The size of the pattern (number of rows of numbers)

    // Loop through each row
    for (int i = 1; i <= n; i++) {
        // Print leading asterisks
        for (int j = 1; j <= n - i; j++) {
            cout << "* ";
        }
        
        // Print the increasing numbers
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        
        // Print the decreasing numbers
        for (int j = i - 1; j >= 1; j--) {
            cout << j << " ";
        }
        
        // Print trailing asterisks
        for (int j = 1; j <= n - i; j++) {
            cout << "* ";
        }
        
        cout << endl;
    }
    	for(int i=4;i>=1;i--){//for the bottom pyramid (row)
		for(int j=4;j>=i;j--){//for the spaces on the left hand size
			cout<<"* ";
		}
		for(int k=1;k<=i;k++){//for the increasing numbers
           
			cout<<k<<" ";
		}
		 for (int k = i - 1; k >= 1; k--) {//for the decreasing numbers(note they shouldnt be printed)
		 	cout<<"  ";
		 }
		 for(int j=i;j<=4;j++){//for the spaces on the left hand size
			cout<<"* ";}
	
		
	
		cout<<endl;
		
	}

    return 0;
}*/

