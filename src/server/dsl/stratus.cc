#include "stratus.hh"

element::element ()
{
        this->operation = EQUAL;
        this->num = 0;
}

element::element (element_operation operation, float state)
{
        this->operation = operation;
        this->num = state;
}

std::list<element>::iterator stratus::add (element elem)
{
        elements.push_back (elem);
        return std::prev (elements.end ());
}

void stratus::remove (std::list<element>::iterator elem)
{
        elements.erase (elem);
        this->evaluate ();
}

void stratus::modify (std::list<element>::iterator elem, element state)
{
        *elem = state;
        this->evaluate ();
}

float stratus::evaluate (float base_state)
{
        for (auto elem : elements)
        {
                switch (elem.operation)
                {
                        case EQUAL:
                                base_state = elem.num;
                                break;
                        case ADD:
                                base_state += elem.num;
                                break;
                        case SUBTRACT:
                                base_state -= elem.num;
                                break;
                        case MULTIPLY:
                                base_state *= elem.num;
                                break;
                        case DIVIDE:
                                base_state /= elem.num;
                                break;
                }
        }
        return base_state;
}
