#ifndef GENERATOR_STORE_CLASS_DEF
#define GENERATOR_STORE_CLASS_DEF
#include "generator.h"
#include <memory>
#include <list>

// Store containg all generators used by particle system
class GeneratorStore
{
public:
    GeneratorStore();
    // adds generator to store
    void AddGenerator(std::shared_ptr<Generator> generator_ptr);
    // all generators will generate light field
    void AllGenerate();
    // remove all generators
    void Clear();
    // set lf texture density for all generators
    void SetDensity(int density);
    // set lf texture resolution for all generators
    void SetResolution(int resolution);
    // returns density used by all generators
    int GetDensity() const;
    // returns resolution used by all generators
    int GetRelution() const;

    const std::vector<std::shared_ptr<Generator>> &Generators;

protected:
    int density = 21;
    int resolution = 5000;
    std::vector<std::shared_ptr<Generator>> generators;
};

#endif