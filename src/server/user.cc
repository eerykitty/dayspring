#include "network.hh"

net::user::user (connection* cxn)
{
        console::t_notify ("NET", "New user " + std::to_string (cxn->id));
        this->cxn = cxn;
}

void net::user::send_message (google::protobuf::MessageLite* msg, uint32_t id)
{
        cxn->queue_message (new message (msg, id));
}

void net::user::flush_messages ()
{
        cxn->flush_message_queue ();
}

void net::user::sync_flag (std::string string)
{
        ige::SetFlag flag_message;
        flag_message.set_flag_name (string);
        flag_message.set_flag_value (flags.is_set (string));
        send_message (&flag_message, MID(flag));
}
