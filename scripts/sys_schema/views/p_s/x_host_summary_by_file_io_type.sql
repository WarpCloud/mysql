-- Copyright (c) 2014, 2021, Oracle and/or its affiliates.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; version 2 of the License.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--
-- View: x$host_summary_by_file_io_type
--
-- Summarizes file IO by event type per host.
--
-- When the host found is NULL, it is assumed to be a "background" thread.
--
-- mysql> select * from x$host_summary_by_file_io_type;
-- +------------+--------------------------------------+-------+---------------+--------------+
-- | host       | event_name                           | total | total_latency | max_latency  |
-- +------------+--------------------------------------+-------+---------------+--------------+
-- | hal1       | wait/io/file/sql/FRM                 |   871 |  168148450470 |  18482624810 |
-- | hal1       | wait/io/file/innodb/innodb_data_file |   173 |  129564287450 |  34087423890 |
-- | hal1       | wait/io/file/innodb/innodb_log_file  |    20 |   77525706960 |  60657475320 |
-- | hal1       | wait/io/file/myisam/dfile            |    40 |    6544493800 |   4580546230 |
-- | hal1       | wait/io/file/mysys/charset           |     3 |    4793558770 |   4713476430 |
-- | hal1       | wait/io/file/myisam/kfile            |    67 |    4384332810 |    300035450 |
-- | hal1       | wait/io/file/sql/ERRMSG              |     5 |    2717434850 |   1687316280 |
-- | hal1       | wait/io/file/sql/pid                 |     3 |     266301490 |    185468920 |
-- | hal1       | wait/io/file/sql/casetest            |     5 |     246814360 |    150193030 |
-- | hal1       | wait/io/file/sql/global_ddl_log      |     2 |      21236410 |     18593640 |
-- | hal2       | wait/io/file/sql/file_parser         |  1422 | 4801104756760 | 135138518970 |
-- | hal2       | wait/io/file/sql/FRM                 |   865 |   85818594810 |   9812303410 |
-- | hal2       | wait/io/file/myisam/kfile            |  1073 |   37143664870 |  15793838190 |
-- | hal2       | wait/io/file/myisam/dfile            |  2991 |   25528215700 |   5252232050 |
-- | hal2       | wait/io/file/sql/dbopt               |    20 |    1067339780 |    153073310 |
-- | hal2       | wait/io/file/sql/misc                |     4 |      59713030 |     33752810 |
-- | hal2       | wait/io/file/archive/data            |     1 |      13907530 |     13907530 |
-- +------------+--------------------------------------+-------+---------------+--------------+
--

CREATE OR REPLACE
  ALGORITHM = MERGE
  DEFINER = 'mysql.sys'@'localhost'
  SQL SECURITY INVOKER 
VIEW x$host_summary_by_file_io_type (
  host,
  event_name,
  total,
  total_latency,
  max_latency
) AS
SELECT IF(host IS NULL, 'background', host) AS host,
       event_name,
       count_star AS total,
       sum_timer_wait AS total_latency,
       max_timer_wait AS max_latency
  FROM performance_schema.events_waits_summary_by_host_by_event_name
 WHERE event_name LIKE 'wait/io/file%'
   AND count_star > 0
 ORDER BY IF(host IS NULL, 'background', host), sum_timer_wait DESC;
