

/**********************************************************************
 *
 *  Declarations for class RWISO8601DateTime
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/iso8601.h#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 **********************************************************************/


#ifndef RW_ISO8601_H
#define RW_ISO8601_H

#include <rw/defs.h>
#include <time.h>

class RW_TOOLS_SYMBOLIC RWISO8601DateTime
{

public:

    enum Format {
        None,
        Date,        // as 2001-09-13
        Time,        // as [T]13:09:33[,312][+01:00]
        DateTime,    // as 2001-09-13T13:09:33[,312][-06:00]
        Unknown      // may be a date as 20010913 or time as 120934
    };

public:

    RWISO8601DateTime(const char*, Format = DateTime);

    void fill(struct tm* t) const;

    bool isValid() const;

    int century() const;
    int decade() const;
    int decade_year() const;
    int year() const;
    int year_day() const;
    int month() const;
    int week() const;
    int week_day() const;
    int day() const;

    int hour() const;
    int minutes() const;
    int seconds() const;
    int milliseconds() const;

    int offset() const;

    bool zone() const;

private:

    // guessing the format of the input string
    static Format format(const char*);

    // parser invocation
    void parse_date(const char*&);
    void parse_full_date(const char*&);
    void parse_dash_date(const char*&);
    void parse_dash_dash_date(const char*&);

    void parse_time(const char*&);
    void parse_hour(const char*&);
    void parse_minutes(const char*&);
    void parse_seconds(const char*&);

    double parse_fraction(const char*&, bool);

    void parse_zone(const char*&);

    void verify(int    /* year */    = -1,
                int    /* century */ = -1,
                int    /* decade */  = -1,
                int    /* dyear */   = -1,
                int    /* yday */    = -1,
                int    /* month */   = -1,
                int    /* mday */    = -1,
                int    /* week */    = -1,
                int    /* wday */    = -1);

private:

    signed int offset_ : 11;       // range [-720,+720]

    unsigned year_     : 12;       // range [1,4095]
    unsigned yday_     :  9;       // range [1,366]

    signed int msec_   : 32;       // range [0-86400000]
};

/**
 * @page iso8601_reference SourcePro ISO 8601 Compatibility Parser Reference
 *
 * @tableofcontents
 *
 * The ISO 8601 specification provides a set of requirements for specifying
 * date, time, and date-time values.
 *
 * The ISO 8601 Compatibility parser can be used by RWDateTime for parsing
 * date, time, and date-time values from strings that are in the ISO 8601
 * format and are known to be compatible with previous versions of SourcePro.
 * This parser was originally used as the default by RWDateTime for parsing
 * ISO 8601 date-time values. The behavior of the parser remains unchanged
 * to ensure compatibility for existing applications. This parser is selected
 * by passing the RWDateTime::iso8601_compat enumerator to the appropriate
 * RWDateTime constructor.
 *
 * The parser may accept invalid formats unexpectedly and produce incorrect
 * results. In debug builds, these formats may produce debug assertions. It
 * is suggested that you use only the date, time, and date-time formats
 * described below.
 *
 * The following sections outline valid date and time formats handled by the
 * parser. Each example assumes that the local timezone is set to UTC, and
 * the current date-time is 2013-05-01T16:28:06,461Z.
 *
 * @section iso8601_date_formats_complete Complete Date Formats
 *
 * <table>
 * <tr><th>Date Format           </th><th>Example                </th><th>Result                       </th></tr>
 * <tr><td><tt>YYYYMMDD     </tt></td><td><tt>18650414      </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-MM-DD   </tt></td><td><tt>1865-04-14    </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYDDD      </tt></td><td><tt>1865104       </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-DDD     </tt></td><td><tt>1865-104      </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYWwwD     </tt></td><td><tt>1865W155      </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-Www-D   </tt></td><td><tt>1865-W15-5    </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * </table>
 *
 * @section iso8601_time_formats_complete Complete And Reduced Precision Time Formats
 *
 * <table>
 * <tr><th>Time Format           </th><th>Example                </th><th>Result                       </th></tr>
 * <tr><td><tt>hhmmss       </tt></td><td><tt>061512        </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss     </tt></td><td><tt>06:15:12      </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm         </tt></td><td><tt>0615          </tt></td><td><tt>2013-05-01T06:15:00Z</tt></td></tr>
 * <tr><td><tt>hh:mm        </tt></td><td><tt>06:15         </tt></td><td><tt>2013-05-01T06:15:00Z</tt></td></tr>
 * <tr><td><tt>hh           </tt></td><td><tt>06            </tt></td><td><tt>2013-05-01T06:00:00Z</tt></td></tr>
 * <tr><td><tt>hhmmss,ss    </tt></td><td><tt>061512,00     </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmmss.ss    </tt></td><td><tt>061512.00     </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss,ss  </tt></td><td><tt>06:15:12,00   </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss.ss  </tt></td><td><tt>06:15:12.00   </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm,mm      </tt></td><td><tt>0615,20       </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm.mm      </tt></td><td><tt>0615.20       </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm,mm     </tt></td><td><tt>06:15,20      </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm.mm     </tt></td><td><tt>06:15.20      </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh,hh        </tt></td><td><tt>06,2533334    </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh.hh        </tt></td><td><tt>06.2533334    </tt></td><td><tt>2013-05-01T06:15:12Z</tt></td></tr>
 * </table>
 *
 * @note All of the above time formats may be prefixed by a single \c T
 * to indicate that a time value is being specified.
 *
 * @note Some complete and reduced precision time formats cannot be reliably
 * parsed as times when specified alone. See the \ref iso8601_avoidance
 * "Parsing for Implied Date and Time Components" section below for more
 * information on avoiding these issues.
 *
 * @section iso8601_zone_formats Zone Designator Formats
 *
 * <table>
 * <tr><th>Zone Offset           </th><th>Example                </th><th>Result                       </th></tr>
 * <tr><td><tt>Z            </tt></td><td><tt>09:54:16Z     </tt></td><td><tt>2013-05-01T09:54:16Z</tt></td></tr>
 * <tr><td><tt>+hh          </tt></td><td><tt>09:54:16+01   </tt></td><td><tt>2013-05-01T08:54:16Z</tt></td></tr>
 * <tr><td><tt>-hh          </tt></td><td><tt>09:54:16-01   </tt></td><td><tt>2013-05-01T10:54:16Z</tt></td></tr>
 * <tr><td><tt>+hhmm        </tt></td><td><tt>09:54:16+0130 </tt></td><td><tt>2013-05-01T08:24:16Z</tt></td></tr>
 * <tr><td><tt>-hhmm        </tt></td><td><tt>09:54:16-0130 </tt></td><td><tt>2013-05-01T11:24:16Z</tt></td></tr>
 * <tr><td><tt>+hh:mm       </tt></td><td><tt>09:54:16+01:30</tt></td><td><tt>2013-05-01T08:24:16Z</tt></td></tr>
 * <tr><td><tt>-hh:mm       </tt></td><td><tt>09:54:16-01:30</tt></td><td><tt>2013-05-01T11:24:16Z</tt></td></tr>
 * </table>
 *
 * @note Negative offsets represent minutes west of UTC, and positive
 * offsets represent minutes east of UTC.
 *
 * @note Negative offsets from UTC only behave correctly when used with a
 * full date-time representation. i.e., <tt>T09:54:16-08:00</tt> does not
 * work, but <tt>2013-04-19T09:54:16-08:00</tt> does.
 *
 * @section iso8601_datetime_formats_complete Complete Date-Time Formats
 *
 * The ISO 8601 specification identifies a complete date-time representation as a
 * complete date representation, a single \c T separator, a complete or reduced
 * precision time representation, and an optional zone designator.
 *
 * @section iso8601_date_formats_reduced Reduced Precision and Truncated Date Formats
 *
 * The ISO 8601 specification allows dates to be expressed in ways that
 * do not indicate a specific point in time. RWDateTime is only able to
 * represent a specific time point. As a result, the reduced precision
 * and truncated date formats must be mapped onto specific time points.
 * The rules used by this parser to do that mapping are covered in the
 * \ref iso8601_implied "Parsing for Implied Date and Time Components"
 * section below. The following table shows the expected result for a
 * few example date values.
 *
 * <table>
 * <tr><th>Date Format           </th><th>Example                </th><th>Result                       </th></tr>
 * <tr><td><tt>YYYY-MM      </tt></td><td><tt>1865-04       </tt></td><td><tt>1865-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY         </tt></td><td><tt>1865          </tt></td><td><tt>1865-05-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY           </tt></td><td><tt>18            </tt></td><td><tt>1813-05-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYMMDD       </tt></td><td><tt>650414        </tt></td><td><tt>2065-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY-MM-DD     </tt></td><td><tt>65-04-14      </tt></td><td><tt>2065-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YYMM        </tt></td><td><tt>-6504         </tt></td><td><tt>2065-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YY-MM       </tt></td><td><tt>-65-04        </tt></td><td><tt>2065-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YY          </tt></td><td><tt>-65           </tt></td><td><tt>2065-05-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MMDD </tt></td><td><tt>-&ndash;0414  </tt></td><td><tt>2013-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MM-DD</tt></td><td><tt>-&ndash;04-14 </tt></td><td><tt>2013-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MM   </tt></td><td><tt>-&ndash;04    </tt></td><td><tt>2013-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;-DD  </tt></td><td><tt>-&ndash;-14   </tt></td><td><tt>2013-05-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYDDD        </tt></td><td><tt>65104         </tt></td><td><tt>2065-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY-DDD       </tt></td><td><tt>65-104        </tt></td><td><tt>2065-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-DDD         </tt></td><td><tt>-104          </tt></td><td><tt>2013-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYWww      </tt></td><td><tt>1865W15       </tt></td><td><tt>1865-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-Www     </tt></td><td><tt>1865-W15      </tt></td><td><tt>1865-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYWww        </tt></td><td><tt>65W15         </tt></td><td><tt>2065-04-06T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YWwwD       </tt></td><td><tt>-5W155        </tt></td><td><tt>2015-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Y-Www-D     </tt></td><td><tt>-5-W15-5      </tt></td><td><tt>2015-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YWww        </tt></td><td><tt>-5W15         </tt></td><td><tt>2015-04-06T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Y-Www       </tt></td><td><tt>-5-W15        </tt></td><td><tt>2015-04-06T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-WwwD        </tt></td><td><tt>-W155         </tt></td><td><tt>2013-04-12T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Www-D       </tt></td><td><tt>-W15-5        </tt></td><td><tt>2013-04-12T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Www         </tt></td><td><tt>-W15          </tt></td><td><tt>2013-04-08T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-W-D         </tt></td><td><tt>-W-5          </tt></td><td><tt>2013-01-04T00:00:00Z</tt></td></tr>
 * </table>
 *
 * @section iso8601_time_formats_truncated Truncated Time Formats
 *
 * The ISO 8601 specification allows times to be expressed with one or
 * more implied time components. This parser does not correctly handle
 * these time formats.
 *
 * @section iso8601_implied Parsing for Implied Date and Time Components
 *
 * Any unspecified date component will have its value determined using
 * the same field from the current date. For example, given the date
 * string <tt>-&ndash;12-12</tt>, the resulting date will be the 12th day of
 * the 12th month of the current year. If the string was passed to the
 * parser on <tt>2013-05-01</tt>, the resulting date would be
 * <tt>2013-12-12</tt>. Similarly, if given the date string <tt>-&ndash;07</tt>,
 * the resulting date would be <tt>2013-07-01</tt>.
 *
 * The use of the current date can result in some unusual behavior.
 * Consider the case in which the current date is <tt>2013-03-31</tt>,
 * and the date string being parsed on that date is <tt>2013-02</tt>.
 * The resulting date would be <tt>2013-02-31</tt> which is invalid.
 * The parser then takes this invalid date and wraps it forward into
 * the next month, resulting in the date <tt>2013-03-03</tt>.
 *
 * Any omitted time component will always get the value zero. As an
 * example, the date-time <tt>2013-04-21T06Z</tt> will be parsed
 * as <tt>2013-04-21T06:00:00Z</tt>.
 *
 * @section iso8601_avoidance Avoiding Parsing Errors
 *
 * Given an input string, the ISO 8601 parser attempts to determine if the
 * input is a date, time or a date-time. If an input could be interpreted
 * as either a date or a time, an attempt will be made to parse the input
 * as a date first. If that fails, an attempt will be made to parse the
 * input as a time. This can make it difficult to use a few of the time
 * formats. Those cases, and appropriate workarounds, are outlined below.
 *
 * - <tt>hhmmss</tt> could be parsed as  <tt>YYMMDD</tt> if the second digit
 * pair could be interpreted as a month (01-12) and the third digit pair
 * could be interpreted as a month day (01-31). Prefix the time string with
 * \c T if you want to specify a time to avoid this issue.
 * - <tt>hhmm</tt> cannot be used with the current interface. It is always
 * treated as <tt>YYYY</tt>. Prefix the time string with \c T to avoid this
 * issue.
 * - <tt>hh</tt> cannot be used with the current interface. It is always
 * treated as <tt>YY</tt>. Prefix the time string with \c T to avoid this
 * issue.
 */

#endif // RW_ISO8601_H
