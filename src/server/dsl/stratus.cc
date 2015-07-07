#include "stratus.hh"

element::element (element_operation operation, element_state state)
{
        this->operation = operation;
        this->state = state;
}

stratus::stratus ()
{
        dirty = false;
}

std::list<element>::iterator element::add (element_operation op, element_state state)
{
        elements.emplace_back (op, state);
        return std::prev (elements.end ());
}

int64_t element::evaluate (element_state* base_state)
{
        for (elements : elem)
        {
                switch (elem.operation)
                {
                        case EQUAL:

                }
        }
}
