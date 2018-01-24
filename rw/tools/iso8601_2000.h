

/**********************************************************************
 *
 *  Declarations for class RWISO8601DateTime_2000
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/iso8601_2000.h#1 $
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


#ifndef RW_ISO8601_2000_H
#define RW_ISO8601_2000_H

#include <rw/defs.h>
#include <time.h>

class RW_TOOLS_SYMBOLIC RWISO8601DateTime_2000
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

    RWISO8601DateTime_2000(const char*, Format = DateTime);

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

    void parse_time(const char*&, bool has_date);
    void parse_hour(const char*&);
    void parse_minutes(const char*&);
    void parse_seconds(const char*&);

    void parse_fraction(const char*&, size_t);

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

    void invalidate(Format);

    void adjust_for_alternate_midnight();

private:

    // not implemented
    RWISO8601DateTime_2000(const RWISO8601DateTime_2000&);

    // not implemented
    RWISO8601DateTime_2000&
    operator=(const RWISO8601DateTime_2000&);

private:

    int offset_;          // range [-720,+720]
    int year_;            // range [0,9999], extended format allows outside this range
    int yday_;            // range [1,366]
    unsigned msec_;       // range [0-86400000]
};

/**
 * @page iso8601_2000_reference SourcePro ISO 8601:2000 Parser Reference
 *
 * @tableofcontents
 *
 * The ISO 8601 specification provides a set of requirements for specifying
 * date, time, and date-time values.
 *
 * The ISO 8601:2000 parser can be used by RWDateTime for parsing date, time,
 * and date-time values from strings compatible with the ISO 8601:2000 format
 * specification. It is selected when the RWDateTime::iso8601_2000 enumerator
 * is passed to the appropriate RWDateTime constructor.
 *
 * The following sections outline valid date and time formats handled by the
 * parser. Each example assumes that the local timezone is set to UTC.
 *
 * @section iso8601_2000_date_formats_complete Complete Date Formats
 *
 * <table>
 * <tr><th>Date Format           </th><th>Example                      </th><th>Result                       </th></tr>
 * <tr><td><tt>YYYYMMDD     </tt></td><td><tt>18650414            </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-MM-DD   </tt></td><td><tt>1865-04-14          </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYDDD      </tt></td><td><tt>1865104             </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-DDD     </tt></td><td><tt>1865-104            </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYWwwD     </tt></td><td><tt>1865W155            </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-Www-D   </tt></td><td><tt>1865-W15-5          </tt></td><td><tt>1865-04-14T00:00:00Z</tt></td></tr>
 * </table>
 *
 * @section iso8601_2000_time_formats_complete Complete And Reduced Precision Time Formats
 *
 * <table>
 * <tr><th>Time Format           </th><th>Example                      </th><th>Result                       </th></tr>
 * <tr><td><tt>hhmmss       </tt></td><td><tt>061512              </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss     </tt></td><td><tt>06:15:12            </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm         </tt></td><td><tt>0615                </tt></td><td><tt>1901-01-01T06:15:00Z</tt></td></tr>
 * <tr><td><tt>hh:mm        </tt></td><td><tt>06:15               </tt></td><td><tt>1901-01-01T06:15:00Z</tt></td></tr>
 * <tr><td><tt>hh           </tt></td><td><tt>06                  </tt></td><td><tt>1901-01-01T06:00:00Z</tt></td></tr>
 * <tr><td><tt>hhmmss,ss    </tt></td><td><tt>061512,00           </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmmss.ss    </tt></td><td><tt>061512.00           </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss,ss  </tt></td><td><tt>06:15:12,00         </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm:ss.ss  </tt></td><td><tt>06:15:12.00         </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm,mm      </tt></td><td><tt>0615,20             </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hhmm.mm      </tt></td><td><tt>0615.20             </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm,mm     </tt></td><td><tt>06:15,20            </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh:mm.mm     </tt></td><td><tt>06:15.20            </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh,hh        </tt></td><td><tt>06,2533334          </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * <tr><td><tt>hh.hh        </tt></td><td><tt>06.2533334          </tt></td><td><tt>1901-01-01T06:15:12Z</tt></td></tr>
 * </table>
 *
 * @note All of the above time formats may be prefixed by a single \c T
 * to indicate that a time value is being specified.
 *
 * @note Some complete and reduced precision time formats cannot be reliably
 * parsed as times when specified alone. See the section entitled
 * \ref iso8601_2000_avoidance "Avoiding Parsing Errors" below for
 * more information on avoiding these issues.
 *
 * @section iso8601_2000_zone_designator Zone Designator Formats
 *
 * <table>
 * <tr><th>Zone Format           </th><th>Example                      </th><th>Result                       </th></tr>
 * <tr><td><tt>Z            </tt></td><td><tt>13:47:42Z           </tt></td><td><tt>1901-01-01T13:47:42Z</tt></td></tr>
 * <tr><td><tt>+hh          </tt></td><td><tt>13:47:42+01         </tt></td><td><tt>1901-01-01T12:47:42Z</tt></td></tr>
 * <tr><td><tt>-hh          </tt></td><td><tt>13:47:42-01         </tt></td><td><tt>1901-01-01T14:47:42Z</tt></td></tr>
 * <tr><td><tt>+hhmm        </tt></td><td><tt>13:47:42+0130       </tt></td><td><tt>1901-01-01T12:17:42Z</tt></td></tr>
 * <tr><td><tt>-hhmm        </tt></td><td><tt>13:47:42-0130       </tt></td><td><tt>1901-01-01T15:17:42Z</tt></td></tr>
 * <tr><td><tt>+hh:mm       </tt></td><td><tt>13:47:42+01:30      </tt></td><td><tt>1901-01-01T12:17:42Z</tt></td></tr>
 * <tr><td><tt>-hh:mm       </tt></td><td><tt>13:47:42-01:30      </tt></td><td><tt>1901-01-01T15:17:42Z</tt></td></tr>
 * </table>
 *
 * @note Negative offsets represent minutes west of UTC, and positive
 * offsets represent minutes east of UTC.
 *
 * @section iso8601_2000_datetime_formats_complete Complete Date-Time Formats
 *
 * The ISO 8601 specification identifies a complete date-time representation
 * as a complete date representation, a single \c T separator, a complete
 * or reduced precision time representation, and an optional zone designator.
 * This parser will correctly handle all date-time representations that
 * follow this format.
 *
 * @section iso8601_2000_date_formats_reduced Reduced Precision and Truncated Date Formats
 *
 * The ISO 8601 specification allows dates to be expressed in ways that
 * do not indicate a specific point in time. RWDateTime is only able to
 * represent a specific time point. As a result, the reduced precision
 * and truncated date formats must be mapped onto specific time points.
 * The rules used by this parser to do that mapping are covered in the
 * \ref iso8601_2000_implied "Parsing for Implied Date and Time Components"
 * section below. The following table shows the expected result for a
 * few example date values.
 *
 * <table>
 * <tr><th>Date Format           </th><th>Example                      </th><th>Result                       </th></tr>
 * <tr><td><tt>YYYY-MM      </tt></td><td><tt>1865-04             </tt></td><td><tt>1865-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY         </tt></td><td><tt>1865                </tt></td><td><tt>1865-01-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY           </tt></td><td><tt>18                  </tt></td><td><tt>1801-01-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYMMDD       </tt></td><td><tt>130414              </tt></td><td><tt>1913-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY-MM-DD     </tt></td><td><tt>13-04-14            </tt></td><td><tt>1913-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YYMM        </tt></td><td><tt>-1304               </tt></td><td><tt>1913-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YY-MM       </tt></td><td><tt>-13-04              </tt></td><td><tt>1913-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YY          </tt></td><td><tt>-13                 </tt></td><td><tt>1913-01-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MMDD </tt></td><td><tt>-&ndash;0414        </tt></td><td><tt>1901-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MM-DD</tt></td><td><tt>-&ndash;04-14       </tt></td><td><tt>1901-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;MM   </tt></td><td><tt>-&ndash;04          </tt></td><td><tt>1901-04-01T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;-DD  </tt></td><td><tt>-&ndash;-14         </tt></td><td><tt>1901-01-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYDDD        </tt></td><td><tt>13104               </tt></td><td><tt>1913-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY-DDD       </tt></td><td><tt>13-104              </tt></td><td><tt>1913-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-DDD         </tt></td><td><tt>-104                </tt></td><td><tt>1901-04-14T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYYWww      </tt></td><td><tt>1865W15             </tt></td><td><tt>1865-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYYY-Www     </tt></td><td><tt>1865-W15            </tt></td><td><tt>1865-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYWwwD       </tt></td><td><tt>13W155              </tt></td><td><tt>1913-04-11T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YY-Www-D     </tt></td><td><tt>13-W15-5            </tt></td><td><tt>1913-04-11T00:00:00Z</tt></td></tr>
 * <tr><td><tt>YYWww        </tt></td><td><tt>13W15               </tt></td><td><tt>1913-04-07T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YWwwD       </tt></td><td><tt>-3W155              </tt></td><td><tt>1903-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Y-Www-D     </tt></td><td><tt>-3-W15-5            </tt></td><td><tt>1903-04-10T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-YWww        </tt></td><td><tt>-3W15               </tt></td><td><tt>1903-04-06T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Y-Www       </tt></td><td><tt>-3-W15              </tt></td><td><tt>1903-04-06T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-WwwD        </tt></td><td><tt>-W155               </tt></td><td><tt>1901-04-12T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Www-D       </tt></td><td><tt>-W15-5              </tt></td><td><tt>1901-04-12T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-Www         </tt></td><td><tt>-W15                </tt></td><td><tt>1901-04-08T00:00:00Z</tt></td></tr>
 * <tr><td><tt>-W-D         </tt></td><td><tt>-W-5                </tt></td><td><tt>1901-01-04T00:00:00Z</tt></td></tr>
 * </table>
 *
 * @section iso8601_2000_time_formats_truncated Truncated Time Formats
 *
 * The ISO 8601 specification allows times to be expressed with one or
 * more implied time components. The rules used by this parser to
 * determine the value for any implied component are discussed in the
 * \ref iso8601_2000_implied "Parsing for Implied Date and Time Components"
 * section below. The following table shows the expected result for a
 * few example time values.
 *
 * <table>
 * <tr><th>Time Format           </th><th>Example                      </th><th>Result                       </th></tr>
 * <tr><td><tt>-mmss        </tt></td><td><tt>-1512               </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm:ss       </tt></td><td><tt>-15:12              </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm          </tt></td><td><tt>-15                 </tt></td><td><tt>1901-01-01T00:15:00Z</tt></td></tr>
 * <tr><td><tt>-&ndash;ss   </tt></td><td><tt>-&ndash;12          </tt></td><td><tt>1901-01-01T00:00:12Z</tt></td></tr>
 * <tr><td><tt>-mmss,ss     </tt></td><td><tt>-1512,00            </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mmss.ss     </tt></td><td><tt>-1512.00            </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm:ss,ss    </tt></td><td><tt>-15:12,00           </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm:ss.ss    </tt></td><td><tt>-15:12.00           </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm,mm       </tt></td><td><tt>-15,20              </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * <tr><td><tt>-mm.mm       </tt></td><td><tt>-15.20              </tt></td><td><tt>1901-01-01T00:15:12Z</tt></td></tr>
 * </table>
 *
 * @note Some truncated time formats cannot be reliably parsed when
 * specified alone. See the \ref iso8601_2000_avoidance
 * "Avoiding Parsing Errors" section below for more information on
 * avoiding these issues.
 *
 * @section iso8601_2000_implied Parsing for Implied Date and Time Components
 *
 * Any implied date or time component will always have its value taken from
 * the epoch time value <tt>1901-01-01T00:00:00</tt>. As an example, the input
 * <tt>2013-04</tt> will be parsed as a date and the resulting date-time value
 * will be <tt>2013-04-01T00:00:00</tt>.
 *
 * @section iso8601_2000_avoidance Avoiding Parsing Errors
 *
 * Given an input string, the ISO 8601:2000 parser attempts to determine if
 * the input is a date, time or a date-time. If an input could be interpreted
 * as either a date or a time, an attempt will be made to parse the input
 * as a date first. If that fails, an attempt will be made to parse the
 * input as a time. This can make it difficult to use a few of the time
 * formats. Those cases, and appropriate workarounds, are outlined below.
 *
 * - <tt>hhmmss</tt> could be parsed as  <tt>YYMMDD</tt> if the second digit
 * pair could be interpreted as a month (01-12) and the third digit pair
 * could be interpreted as a month day (01-31). Prefix the time string with
 * \c T or use a zone designator to avoid the problem.
 * - <tt>-mmss</tt> could be parsed as  <tt>-YYMM</tt> if the second digit
 * pair could be interpreted as a month (01-12). Use a zone designator to
 * avoid this issue.
 * - <tt>-&ndash;ss</tt> could be parsed as  <tt>-&ndash;MM</tt> if the digit
 * pair could be interpreted as a month (01-12). Use a zone designator to
 * avoid this issue.
 * - <tt>-mm</tt> cannot be used with the current interface. It is always
 * treated as <tt>-YY</tt>. Use a zone designator to avoid the problem.
 * - <tt>hhmm</tt> cannot be used with the current interface. It is always
 * treated as <tt>YYYY</tt>. Prefix the time string with \c T or use a
 * zone designator to avoid this issue.
 * - <tt>hh</tt> cannot be used with the current interface. It is always
 * treated as <tt>YY</tt>. Prefix the time string with \c T or use a time
 * zone designator to avoid the problem.
 */

#endif // RW_ISO8601_2000_H
