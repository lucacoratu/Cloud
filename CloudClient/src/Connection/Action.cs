using System;
using System.Collections.Generic;
using System.Text;


namespace CloudClient
{
    enum Action
    {
        NO_ACTION = 0,
        UNKNOWN_ACTION = 1,
        RECEIVE_PUBLIC_KEY = 2,
        TEST_CONNECTION = 3,
        REGISTER_ACCOUNT = 4,
        LOGIN_INTO_ACCOUNT = 5,
    };

}