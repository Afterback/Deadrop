#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace deadrop
{
    class BinaryFile
    {
    public:

        // default constructor
        BinaryFile() = default;

        // opens the file for reading and writing, creates a new file if create_file is true
        bool OpenFile(const std::string& file, bool create_file)
        {
            if (create_file)
            {
                stream.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
            }
            else
            {
                stream.open(file, std::fstream::in | std::fstream::out | std::fstream::binary);
            }

            // check if the file was opened successfully
            if (!stream.is_open())
            {
                return false;
            }

            return true;
        }

        // returns whether the file is open or not
        bool IsOpen()
        {
            return stream.is_open();
        }

        // writes a type to the file, does not check if the file is open
        template<typename T>
        void Write(T& t)
        {
            stream.write(reinterpret_cast<char*>(&t), sizeof(t));
        }

        // NOTE: use BinaryFile::Write(vec) not Write<std::vector<T>>(myvec)
        // since that will call the Write<T> overload!
        // writes the content of the vector to the file as it is in memory,
        // does not check if the file is open
        template<typename T>
        void Write(std::vector<T>& vec)
        {
            size_t size = vec.size();
            if (size < 0) return;

            stream.write(reinterpret_cast<char*>(&vec[0]), size * sizeof(T));
        }

        // reads a type from the file, does not check if the file is open
        template<typename T>
        void Read(T& t)
        {
            stream.read(reinterpret_cast<char*>(&t), sizeof(t));
        }

        // reads a vector from the file, does not check if the file is open
        template<typename T>
        void Read(std::vector<T>& vec, int size)
        {
            T temp;
            if (size < 0) return;
            for (int i = 0; i < size; i++)
            {
                stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
                vec.push_back(temp);
            }
        }

        // moves the reading postion 'pos' times from the specified direction ios_base::beg, cur, end
        void Seek(unsigned int pos, std::ios_base::seekdir dir)
        {
            stream.seekg(pos, dir);
        }

        // closes the file
        void Close()
        {
            stream.close();
        }
    private:
        std::fstream stream;
    };
}