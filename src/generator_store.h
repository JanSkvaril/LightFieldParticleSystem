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
    const std::list<std::shared_ptr<Generator>> &Generators;

protected:
    std::list<std::shared_ptr<Generator>> generators;
};

#endif