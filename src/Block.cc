#include "Block.hh"

#include "BlockSpecification.hh"

bool Block::IsTransparent() const
{
    return GetBlockSpecificationManager()->GetSpecification(m_Type).IsTransparent();
}
