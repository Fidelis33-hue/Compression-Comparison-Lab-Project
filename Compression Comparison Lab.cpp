// PROGRAM: COMPARING RUN-LENGTH ENCODING AND DELTA ENCODING

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>


std::string readTextFile (std::string filename)
{
    std::ifstream file (filename);
    std::string text, line;

    if (!file)
    {
        std::cout << "Error. Please try again.";
        return "";
    }

    while (getline (file,line))
    {
        // to prevent new lines in the file from getting lost
        text+= line+ '\n';
    }

    file.close ();
    return text;
}



std::vector <int> readLogFile (std::string filename)
{
    std::ifstream file (filename);
    std::vector <int> logValues;
    int value;

    if (!file)
    {
        std::cout << "Error. Please try again.\n";
        return logValues;
    }

    while (file >> value)
    {
        logValues.push_back (value);
    }

    file.close ();
    return logValues;
}



// Converts characters to ASCII values
// This is necessary because delta encoding operates on numerical numbers
std::vector <int> AsciiValues (std::string text)
{
    std::vector <int> asciiValues;

    for (size_t i = 0; i < text.length (); i++)
    {
        // Converts each character in the text to ASCII values and stores it
        asciiValues.push_back (int (text[i]));
    }

    return asciiValues;
}


// Calculates the difference between the ASCII values of the text
std::vector <int> deltaEncode (std::vector <int> asciiValues)
{

    std::vector <int> deltaEncoded;

    // Safety check; to avoid accessing an empty vector
    if  (asciiValues.size()==0)
        return deltaEncoded;

    // Stores the first ASCII value as it is
    deltaEncoded.push_back (asciiValues [0]);

    for (size_t i = 1; i < asciiValues.size (); i++)
    {
       // Computes the differences between the remaining ASCII values and stores them
        deltaEncoded.push_back (asciiValues [i]- asciiValues [i-1]);
    }

    return deltaEncoded;
}


// RLE COMPRESSION FUNCTION
// Performs run-length encoding on the input text
// Input: A string of characters
// Output: A vector of character and count pair

std::vector <std::pair <int,int>> rleCompression (std::vector <int> deltaEncoded)
{
     // To store the result after compressing
    std::vector < std::pair <int, int>> compressed;

    // Safety Check
    if (deltaEncoded.size() == 0)
    return compressed;

    // Initialize with the first character in the text
    int currentValue = deltaEncoded [0];
    int count = 1;

    // Start from the next character to compare the with the previous one
    for (size_t i = 1; i < deltaEncoded.size(); i++ )
    {
        // Increases the count if the character is the same with the previous one
        if (deltaEncoded [i] == currentValue )
        {
            count++;
        }
        else
        {
            // Saves the completed run
            compressed.push_back ({currentValue,count});

           // Starts new run with a new character
            currentValue = deltaEncoded [i];
            count = 1;
        }
    }

    //Store the final character after the last loop
    compressed.push_back ({currentValue,count});
    return compressed;
}

// Compares the original size of the data to the compressed one
// Takes the size before compression and after compression
double compressionRatio (int originalSize, int compressedSize)
{
     // Safety Check
     // If compressed size is zero,
     //return 0 to avoid crashing
     if (compressedSize == 0)
        return 0;

    double Ratio =  (double) originalSize / compressedSize;

   return Ratio;
}



void DisplayDelta (std::vector <int> deltaEncoded)
{
    for (size_t i = 0; i < deltaEncoded.size (); i++)
    {
        std::cout << deltaEncoded [i] << " ";
    }
    std::cout << "\n";
}


void DisplayRLE (std::vector <std::pair <int, int>> compressed)
{
    for (size_t i = 0; i < compressed.size (); i++)
    {
        std::cout << "(" << compressed [i].first << "," << compressed [i].second << ")" << " ";
    }
    std::cout << "\n";
}


void displayCompressionRatio (std::string name, int originalSize, int compressedSize)
{
    double compressedRatioResults = compressionRatio (originalSize, compressedSize);

    std::cout << "COMPRESSION RATIO RESULTS \n";
    std::cout << name << ":" << compressedRatioResults << "\n";
}


// To handle user interaction
void getUserInput ()
{
    std::string filename;
    int choice;

    std::cout << "1. Compress Text File.\n";
    std::cout << "2.Compress file with numerical values.\n\n";
    std::cout << "Enter choice: \n";
    std::cin >> choice;

    // Safety check
    if (choice !=1 && choice!=2)
    {
        std::cout << "Error. Please enter a valid choice.";
        return;
    }

    std::cout << "\nEnter file.\n";
    std::cin >> filename;


    if (choice ==  1)
    {
     std::string text = readTextFile (filename);

     int originalSize = text.length();;
     std::vector <int> asciiResults = AsciiValues (text);
     std::vector <int> deltaEncodedResults = deltaEncode (asciiResults);
     std::cout << "      DELTA ENCODING RESULTS    \n";
     DisplayDelta (deltaEncodedResults);

     std::vector <std::pair <int, int>> rleResults = rleCompression (deltaEncodedResults);
     std::cout << "      RUN-LENGTH ENCODING RESULTS        \n";
     DisplayRLE (rleResults);
     std::cout << "\n\n";

     // Displaying the Compression ratio ;
     double compressedSize = rleResults.size() *2;
      std::cout << "Original Size: " << originalSize << "\n";
        std::cout << "Compressed Size: " << compressedSize << "\n";
     displayCompressionRatio ("COMPRESSION RATIO: ", originalSize, compressedSize);
    }
    else
    {
        std::vector <int> logData = readLogFile (filename);
        int originalSize = logData.size ();
        std::vector <int> deltaEncodedResults = deltaEncode (logData);
        std::cout << "       DELTA ENCODING RESULTS      \n";
        DisplayDelta (deltaEncodedResults);
        std::cout << "\n\n";

         std::vector <std::pair <int, int>>  rleResults = rleCompression (deltaEncodedResults);
         std::cout << "    RUN-LENGTH ENCODING RESULTS  \n";
         DisplayRLE (rleResults);

        // Displaying the Compression Ratio
    double compressedSize = rleResults.size() * 2;
     std::cout << "Original Size: " << originalSize << "\n";
        std::cout << "Compressed Size: " << compressedSize << "\n";
    displayCompressionRatio ("COMPRESSION RATIO: ", originalSize, compressedSize);
    }

}


int main ()
{
    std::cout << "===========================================================\n";
    std::cout << "       IMPLEMENTING RUN-LENGTH ENCODING AND DELTA ENCODING   \n";
    std::cout << "===========================================================\n\n";

    getUserInput ();

    return 0;
}



