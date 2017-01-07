#pragma once

#include "cereal/archives/json.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/cereal.hpp"

#include "templatemetadata.h"

namespace geometrize
{

namespace serialization
{

template<class Archive>
void save(Archive& archive, const TemplateMetadata& m)
{
    archive(m.x, m.y, m.z);
}

template<class Archive>
void load(Archive& archive, TemplateMetadata& m)
{
    archive(m.x, m.y, m.z);
}

}

}
