#ifndef GENERATOR_STORE_CLASS_DEF
#define GENERATOR_STORE_CLASS_DEF
#include "generator.h"
#include <memory>
#include <list>
class GeneratorStore
{
public:
    GeneratorStore();
    void AddGenerator(std::shared_ptr<Generator> generator_ptr);
    void AllGenerate();
    void Clear();
    void SetDensity(int density);
    void SetResolution(int resolution);
    const std::list<std::shared_ptr<Generator>> &Generators;
    int GetDensity() const;
    int GetRelution() const;

protected:
    int density = 21;
    int resolution = 5000;
    std::list<std::shared_ptr<Generator>> generators;
};

#endif