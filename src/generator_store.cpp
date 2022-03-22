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

void GeneratorStore::SetDensity(int density)
{
    for (auto &generator : generators)
    {
        generator->ChangeDensity(density);
    }
    this->density = density;
}

void GeneratorStore::SetResolution(int resolution)
{
    for (auto &generator : generators)
    {
        generator->ChangeResolution(resolution);
    }
    this->resolution = resolution;
}

int GeneratorStore::GetDensity() const
{
    return density;
}

int GeneratorStore::GetRelution() const
{
    return resolution;
}
