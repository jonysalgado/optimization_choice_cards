# ifndef UTILS_CSV_H
# define UTILS_CSV_H

#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
// tuple<vector<pair<string, vector<float>>>, vector<int>>
vector<pair<string, vector<int>>> read_csv(string filename)
{
    ifstream dataset(filename);

    // Create a vector of <string, int vector> pairs to store the result
    vector<pair<string, vector<int>>> result;
    string line, colname;
    float val;
    if(dataset.good())
    {
        // Extract the first line in the file
        getline(dataset, line);
        // Create a stringstream from line
        stringstream ss(line);
        
        if(ss.peek() == ',') ss.ignore();
        // Extract each column name
        while(getline(ss, colname, ',')){
            // cout << colname << endl;
            // Initialize and add <colname, in            cout << colname << endl;t vector> pairs to result
            result.push_back({colname, vector<int> {}});
        }
    }
    int size = result.size();
    // Read data, line by line
    while(getline(dataset, line))
    {
        // Create a stringstream of the current line
        stringstream ss(line);
        
        // Keep track of the current column index
        int index = 0;
        // Extract each integer
        while(index < size){
            ss >> val;
            // Add the current integer to the 'index' column's values vector
            result.at(index).second.push_back(val);
            
            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
            
            // Increment the column index
            index++;
        }
        // ss >> val;
        // name.push_back((int)val);
    }

    // Close file
    dataset.close();
    return result;
}


void
write_csv(vector<vector<int>> best_choices,
    vector<int> best_results, string filename)
{
    ofstream output(filename);

    for(int i = 0; i < best_choices.size(); i++)
    {
        for(int j = 0; j < best_choices[j].size(); j++){
            output << best_choices[i][j];
            output << ",";
        }
        output << best_results[i];
        output << "\n";
    }
    // Close the file
    output.close();

}

void
write_csv_(vector<pair<string, vector<float>>> result,
    vector<int> name, string filename)
{
    ofstream output(filename);

    for(int j = 0; j < result.size(); j++)
    {
        output << result.at(j).first;
        if(j != result.size() - 1) 
            output << ","; // No comma at end of line
    }
    output << "\n";
    
    //Send data to the stream
    for(int i = 0; i < result.at(0).second.size(); ++i)
    {
        for(int j = 0; j < result.size(); ++j)
        {
            if(j < result.size() - 1){
                output << result.at(j).second.at(i); 
                output << ","; 
            }
            else
                output << name.at(i);
        }
        output << endl;
    }
    
    // Close the file
    output.close();
}

# endif