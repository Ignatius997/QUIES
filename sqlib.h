#ifndef SQLIB_H
#define SQLIB_H

#define SQLIB_OUT_EXTENSION ".out"
#define SQLIB_PLAN_EXTENSION ".plan"
#define SQLIB_PLAN_CREATION_EXTENSION "_plancreation.sql"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

std::string& sql_username() {
    static std::string* username = new std::string();
    return *username;
}

void sql_free_username() {
    if (&sql_username() != nullptr) delete &sql_username();
}

std::vector<std::string>& sql_tabs() {
    static std::vector<std::string>* tabs = new std::vector<std::string>();
    return *tabs;
}

void sql_free_tabs() {
    if (&sql_tabs() != nullptr) delete &sql_tabs();
}

void sql_spool_on(const std::string& s) { std::cout << "SPOOL " << s << std::endl; }
void sql_spool_off() { std::cout << "SPOOL OFF" << std::endl; }

void sql_run(const std::string& s) { std::cout << "@" << s << std::endl; }

void sql_stats_del(const std::string& tab) {
    std::cout << "BEGIN " << std::endl
              << "DBMS_STATS.DELETE_TABLE_STATS('" << sql_username() << "', '" << tab << "', NULL);" << std::endl
              << "END;\n/" << std::endl;
}

void sql_idxs_del(const std::string& tab) {
   std::cout << "BEGIN\n"
             << "    FOR ind IN (SELECT index_name FROM user_indexes WHERE table_name = '" << tab << "') LOOP\n"
             << "        EXECUTE IMMEDIATE 'DROP INDEX ' || ind.index_name;\n"
             << "    END LOOP;\n"
             << "END;\n/" << std::endl;
}

void sql_file_del(const std::string& command) {
    std::cout << "host rm -f " << command << std::endl;
}

void sql_outfile_del(const std::string& command) {
    sql_file_del(command + SQLIB_OUT_EXTENSION);
}

void sql_planfile_del(const std::string& command) {
    sql_file_del(command + SQLIB_PLAN_EXTENSION);
}

void sql_timing_on() { std::cout << "SET TIMING ON" << std::endl; }
void sql_timing_off() { std::cout << "SET TIMING OFF" << std::endl; }

void sql_display_prep() {
    std::cout << "SET LINESIZE 200" << std::endl
              << "SET PAGESIZE 100" << std::endl;
}

void sql_append_to_file(const std::string& file, const std::string& text) {
    std::cout << "host echo \"" << text << "\" >> " << file << std::endl;
}

void sql_copy_file_to_file(const std::string& tgt, const std::string& src) {
    std::cout << "host cat " << src << " >> " << tgt << std::endl;
}

void sql_display_plan(const std::string& command) {
    sql_display_prep();
    std::string og_file = command + ".sql";
    std::string new_file = command + SQLIB_PLAN_CREATION_EXTENSION;

    sql_file_del(new_file);
    sql_append_to_file(new_file, "EXPLAIN PLAN FOR ");
    sql_copy_file_to_file(new_file, og_file);

    sql_run(new_file);
    sql_spool_on(command + SQLIB_PLAN_EXTENSION);
    std::cout << "SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());" << std::endl;
    sql_spool_off();

    sql_file_del(new_file);
}

void sql_sleep(double t) { std::cout << "host sleep " << t << std::endl; }

void sql_run_and_save_time(const std::string& command) {
    sql_outfile_del(command);
    for (auto tab : sql_tabs()) {
        sql_stats_del(tab);
        sql_idxs_del(tab);
    }

    sql_timing_on();
    sql_spool_on(command + SQLIB_OUT_EXTENSION);
    sql_run(command);
    sql_timing_off();
    sql_spool_off();
}

void sql_run_and_save_plan(const std::string& command) {
    sql_planfile_del(command);
    for (auto tab : sql_tabs()) {
        sql_stats_del(tab);
        sql_idxs_del(tab);
    }

    sql_spool_on(command + SQLIB_PLAN_EXTENSION);
    sql_display_plan(command);
}

/** Parameter `command` should not have any extension. */
void sql_full_run(const std::string& command) {
    // delete files
    sql_outfile_del(command);
    sql_planfile_del(command);
    for (auto tab : sql_tabs()) {
        sql_stats_del(tab);
        sql_idxs_del(tab);
    }

    // out
    sql_timing_on();
    sql_spool_on(command + SQLIB_OUT_EXTENSION);
    sql_run(command);
    sql_timing_off();
    sql_spool_off();

    // plan
    sql_display_plan(command);
}

#endif // SQLIB_H
