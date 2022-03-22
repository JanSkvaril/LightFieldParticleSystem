#include "generator_store.h"

GeneratorStore::GeneratorStore() : Generators(generators)
{
}

void GeneratorStore::AddGenerator(std::shared_ptr<Generator> generator_ptr)
{
    generators.push_back(generator_ptr);
}

void GeneratorStore::AllGenerate()
{
    for (auto &generator : generators)
    {
        generator->Generate();
    }
}

void GeneratorStore::Clear()
{
    generators.clear();
}
