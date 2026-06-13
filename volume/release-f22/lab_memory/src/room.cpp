/**
 * @file room.cpp
 * Implementation of the Room class.
 */

#include <algorithm>
#include <iostream>
#include "room.h"

Room::Room()
    : capacity(0), count(0), max_letters(26), letters(NULL), letterCount(0)
{
    letters = new Letter[max_letters];
}

Room::Room(const std::string& init_name, int init_capacity)
    : name(init_name),
      capacity(init_capacity),
      count(0),
      max_letters(26),
      letterCount(0)
{
    letters = new Letter[max_letters];
}

Room::Room(const Room& other)
{
    copy(other);
}

Room& Room::operator=(const Room& other)
{
    if (this != &other) {
        clear();
        copy(other);
    }
    return *this;
}

Room::~Room()
{
    //printf("deleting room: %p\nletters: %p\n", (void*)this, (void*)this->letters);
    clear();
}

void Room::addLetter(const Letter& L)
{
    //printf("accessing room: %p\nletters: %p\n", (void*)this, (void*)this->letters);
    letters[letterCount++] = L;
    count += L.count;
}

int Room::spaceRemaining()
{
    return capacity - count;
}

void Room::print(std::ostream & stream /* = std::cout */)
{
    stream << name << " (" << count << "/" << capacity << "):";
    for (int L = 0; L < letterCount; L++)
        stream << " " << letters[L].letter;
    stream << std::endl;
}

void Room::clear()
{
    if (letters != NULL)
        delete[] letters;
    letters = NULL;
}

void Room::copy(const Room& other)
{
    //printf("copying: %p -> %p\n", (void*)&other, (void*)this);
    name = other.name;
    capacity = other.capacity;
    count = other.count;
    letterCount = other.letterCount;
    letters = new Letter[max_letters]{};
    std::copy_n(other.letters, other.letterCount, letters);

}
