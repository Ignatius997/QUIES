#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "sqlib.h"

using namespace std;

int main(int argc, char* argv[]) {
    // set username. Sqlplus username should be sth like ab12345
    assert(argc == 2);
    sql_username() = argv[1];

    int TAB_COUNT;
    int TASK_NO;
    int SUBTASKS_COUNT;
    int MODIFICATIONS_COUNT;
    vector<string> commands;

    cerr << "Tables count: "; cin >> TAB_COUNT;

    cerr << "Tables names: ";
    for (int i = 0; i < TAB_COUNT; ++i) {
        string tab; cin >> tab;
        sql_tabs().push_back(tab);
    }

    cerr << "Task number: "; cin >> TASK_NO;
    cerr << "Subtasks count: "; cin >> SUBTASKS_COUNT;
    cerr << "Different solves count: "; cin >> MODIFICATIONS_COUNT;

    for (int i = 0; i < SUBTASKS_COUNT; ++i) {
        string com = to_string(TASK_NO) + (char)('a' + i);
        commands.push_back(com);
    }

    for (auto it = commands.begin(); it != commands.end(); ++it) {
        string command = *it;
        sql_full_run(command);

        for (int i = 0; i < MODIFICATIONS_COUNT - 1; ++i) {
            command = command + command.back();
            sql_full_run(command);
        }
    }

    sql_free_username();
    sql_free_tabs();
}