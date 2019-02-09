/* Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef _mysql_sql_command_h
#define _mysql_sql_command_h

/*
  @enum  enum_sql_command
  @brief SQL Commands

         SQL Command is resolved during SQL parsing and assigned to the Lex
         object, accessible from the THD.

         When a command is added here, be sure it's also added in mysqld.cc
         in "struct show_var_st status_vars[]= {" ...

         If the command returns a result set or is not allowed in stored
         functions or triggers, please also make sure that
         sp_get_flags_for_command (sp_head.cc) returns proper flags for the
         added SQLCOM_.
*/
enum enum_sql_command {
  SQLCOM_SELECT,
  SQLCOM_CREATE_TABLE,
  SQLCOM_CREATE_INDEX,
  SQLCOM_ALTER_TABLE,
  SQLCOM_UPDATE,
  SQLCOM_INSERT,
  SQLCOM_INSERT_SELECT,
  SQLCOM_DELETE,
  SQLCOM_TRUNCATE,
  SQLCOM_DROP_TABLE,
  SQLCOM_DROP_INDEX,
  SQLCOM_SHOW_DATABASES,
  SQLCOM_SHOW_TABLES,
  SQLCOM_SHOW_FIELDS,
  SQLCOM_SHOW_KEYS,
  SQLCOM_SHOW_VARIABLES,
  SQLCOM_SHOW_STATUS,
  SQLCOM_SHOW_ENGINE_LOGS,
  SQLCOM_SHOW_ENGINE_STATUS,
  SQLCOM_SHOW_ENGINE_MUTEX,
  SQLCOM_SHOW_PROCESSLIST,
  SQLCOM_SHOW_MASTER_STAT,
  SQLCOM_SHOW_SLAVE_STAT,
  SQLCOM_SHOW_GRANTS,
  SQLCOM_SHOW_CREATE,
  SQLCOM_SHOW_CHARSETS,
  SQLCOM_SHOW_COLLATIONS,
  SQLCOM_SHOW_CREATE_DB,
  SQLCOM_SHOW_TABLE_STATUS,
  SQLCOM_SHOW_TRIGGERS,
  SQLCOM_LOAD,
  SQLCOM_SET_OPTION,
  SQLCOM_LOCK_TABLES,
  SQLCOM_UNLOCK_TABLES,
  SQLCOM_GRANT,
  SQLCOM_CHANGE_DB,
  SQLCOM_CREATE_DB,
  SQLCOM_DROP_DB,
  SQLCOM_ALTER_DB,
  SQLCOM_REPAIR,
  SQLCOM_REPLACE,
  SQLCOM_REPLACE_SELECT,
  SQLCOM_CREATE_FUNCTION,
  SQLCOM_DROP_FUNCTION,
  SQLCOM_REVOKE,
  SQLCOM_OPTIMIZE,
  SQLCOM_CHECK,
  SQLCOM_ASSIGN_TO_KEYCACHE,
  SQLCOM_PRELOAD_KEYS,
  SQLCOM_FLUSH,
  SQLCOM_KILL,
  SQLCOM_ANALYZE,
  SQLCOM_ROLLBACK,
  SQLCOM_ROLLBACK_TO_SAVEPOINT,
  SQLCOM_COMMIT,
  SQLCOM_SAVEPOINT,
  SQLCOM_RELEASE_SAVEPOINT,
  SQLCOM_SLAVE_START,
  SQLCOM_SLAVE_STOP,
  SQLCOM_START_GROUP_REPLICATION,
  SQLCOM_STOP_GROUP_REPLICATION,
  SQLCOM_BEGIN,
  SQLCOM_CHANGE_MASTER,
  SQLCOM_CHANGE_REPLICATION_FILTER,
  SQLCOM_RENAME_TABLE,
  SQLCOM_RESET,
  SQLCOM_PURGE,
  SQLCOM_PURGE_BEFORE,
  SQLCOM_SHOW_BINLOGS,
  SQLCOM_SHOW_OPEN_TABLES,
  SQLCOM_HA_OPEN,
  SQLCOM_HA_CLOSE,
  SQLCOM_HA_READ,
  SQLCOM_SHOW_SLAVE_HOSTS,
  SQLCOM_DELETE_MULTI,
  SQLCOM_UPDATE_MULTI,
  SQLCOM_SHOW_BINLOG_EVENTS,
  SQLCOM_DO,
  SQLCOM_SHOW_WARNS,
  SQLCOM_EMPTY_QUERY,
  SQLCOM_SHOW_ERRORS,
  SQLCOM_SHOW_STORAGE_ENGINES,
  SQLCOM_SHOW_PRIVILEGES,
  SQLCOM_HELP,
  SQLCOM_CREATE_USER,
  SQLCOM_DROP_USER,
  SQLCOM_RENAME_USER,
  SQLCOM_REVOKE_ALL,
  SQLCOM_CHECKSUM,
  SQLCOM_CREATE_PROCEDURE,
  SQLCOM_CREATE_SPFUNCTION,
  SQLCOM_CALL,
  SQLCOM_DROP_PROCEDURE,
  SQLCOM_ALTER_PROCEDURE,
  SQLCOM_ALTER_FUNCTION,
  SQLCOM_SHOW_CREATE_PROC,
  SQLCOM_SHOW_CREATE_FUNC,
  SQLCOM_SHOW_STATUS_PROC,
  SQLCOM_SHOW_STATUS_FUNC,
  SQLCOM_PREPARE,
  SQLCOM_EXECUTE,
  SQLCOM_DEALLOCATE_PREPARE,
  SQLCOM_CREATE_VIEW,
  SQLCOM_DROP_VIEW,
  SQLCOM_CREATE_TRIGGER,
  SQLCOM_DROP_TRIGGER,
  SQLCOM_XA_START,
  SQLCOM_XA_END,
  SQLCOM_XA_PREPARE,
  SQLCOM_XA_COMMIT,
  SQLCOM_XA_ROLLBACK,
  SQLCOM_XA_RECOVER,
  SQLCOM_SHOW_PROC_CODE,
  SQLCOM_SHOW_FUNC_CODE,
  SQLCOM_ALTER_TABLESPACE,
  SQLCOM_INSTALL_PLUGIN,
  SQLCOM_UNINSTALL_PLUGIN,
  SQLCOM_BINLOG_BASE64_EVENT,
  SQLCOM_SHOW_PLUGINS,
  SQLCOM_CREATE_SERVER,
  SQLCOM_DROP_SERVER,
  SQLCOM_ALTER_SERVER,
  SQLCOM_CREATE_EVENT,
  SQLCOM_ALTER_EVENT,
  SQLCOM_DROP_EVENT,
  SQLCOM_SHOW_CREATE_EVENT,
  SQLCOM_SHOW_EVENTS,
  SQLCOM_SHOW_CREATE_TRIGGER,
  SQLCOM_ALTER_DB_UPGRADE,
  SQLCOM_SHOW_PROFILE,
  SQLCOM_SHOW_PROFILES,
  SQLCOM_SIGNAL,
  SQLCOM_RESIGNAL,
  SQLCOM_SHOW_RELAYLOG_EVENTS,
  SQLCOM_GET_DIAGNOSTICS,
  SQLCOM_ALTER_USER,
  SQLCOM_EXPLAIN_OTHER,
  SQLCOM_SHOW_CREATE_USER,
  SQLCOM_SHUTDOWN,
  SQLCOM_ALTER_INSTANCE,
  SQLCOM_CHANGE_EFFECTIVE_USER,
  /* This should be the last !!! */
  SQLCOM_END
};

#endif /* _mysql_sql_command_h */
