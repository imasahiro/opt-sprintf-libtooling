enum format_flag {
  /* length */
  HALF = 1 << 0,
  HALF_HALF = 1 << 1,
  LONG = 1 << 2,
  LONG_LONG = 1 << 3,
  LONG_DOUBLE = 1 << 4,

  /* flag */
  SHARP = 1 << 5,
  SPACE = 1 << 6,
  ZERO = 1 << 7,
  PLUS = 1 << 8,

  /* have precision */
  PRECISION = 1 << 9,

  /* print lower case charctor */
  LOWER_CASE = 1 << 10,

  /* type */
  PRINT_HEX = 1 << 11,
  PRINT_INT = 1 << 12,
  PRINT_UINT = 1 << 13,
  PRINT_POINTER = 1 << 14,
  PRINT_STRING = 1 << 15,
  PRINT_CHAR = 1 << 16,
  PRINT_DOUBLE_E = 1 << 17,
  PRINT_DOUBLE_F = 1 << 18,
  PRINT_DOUBLE_G = 1 << 19,
  PRINT_DOUBLE_HEX = 1 << 20,
  PRINT_PERCENT = 1 << 21,

  PRINT_CONST_STRING = 1 << 22,
  ERROR = -1
};

struct FormatArgument {
  unsigned flag_;
  int length_;
  int width_;
  int precision_;
  const char *data_;
public:
  FormatArgument(unsigned flag, int length, int width, int precision) :
    flag_(flag), length_(length), width_(width),
    precision_(precision), data_(NULL) {}

  FormatArgument(int length, const char *data) :
    flag_(PRINT_CONST_STRING), length_(length), width_(0),
    precision_(0), data_(data) {}

private:
  static inline bool IS_FLAG(unsigned v, enum format_flag flag) {
    return (((v) & flag) == flag);
  }
public:
  bool Sharp() const { return IS_FLAG(flag_, SHARP); }
  bool Space() const { return IS_FLAG(flag_, SPACE); }
  bool Plus() const { return IS_FLAG(flag_, PLUS); }
  bool Zero() const { return IS_FLAG(flag_, ZERO); }
  bool Precision() const { return IS_FLAG(flag_, PRECISION); }

  bool LowerCase() const { return IS_FLAG(flag_, LOWER_CASE); }

  bool Half() const { return IS_FLAG(flag_, HALF); }
  bool HalfHalf() const { return IS_FLAG(flag_, HALF_HALF); }
  bool Long() const { return IS_FLAG(flag_, LONG); }
  bool LongLong() const { return IS_FLAG(flag_, LONG_LONG); }
  bool LongDouble() const { return IS_FLAG(flag_, LONG_DOUBLE); }

  bool SInt() const { return IS_FLAG(flag_, PRINT_INT); }
  bool UInt() const { return IS_FLAG(flag_, PRINT_UINT); }
  bool Hex() const { return IS_FLAG(flag_, PRINT_HEX); }
  bool Percent() const { return IS_FLAG(flag_, PRINT_PERCENT); }
  bool Pointer() const { return IS_FLAG(flag_, PRINT_POINTER); }
  bool String() const { return IS_FLAG(flag_, PRINT_STRING); }
  bool Char() const { return IS_FLAG(flag_, PRINT_CHAR); }
  bool DoubleE() const { return IS_FLAG(flag_, PRINT_DOUBLE_E); }
  bool DoubleF() const { return IS_FLAG(flag_, PRINT_DOUBLE_F); }
  bool DoubleG() const { return IS_FLAG(flag_, PRINT_DOUBLE_G); }
  bool DoubleA() const { return IS_FLAG(flag_, PRINT_DOUBLE_HEX); }
};

struct SprintfOptimizer {
  int check_size_;
  SprintfOptimizer(int check_size) : check_size_(check_size) {}
private:
  void qrintf_x(std::string *out, FormatArgument &fmt, std::string arg, int zero, int lower) {
    *out = "_qrintf_chk_x(" + *out + ", " + arg + ")";
  }
  void qrintf_hx(std::string *out, FormatArgument &fmt, std::string arg, int zero, int lower)
  {
    *out = "_qrintf_chk_hx(" + *out + ", " + arg + ")";
  }
  void qrintf_lx(std::string *out, FormatArgument &fmt, std::string arg, int zero, int lower)
  {
    *out = "_qrintf_chk_lx(" + *out + ", " + arg + ")";
  }
  void qrintf_llx(std::string *out, FormatArgument &fmt, std::string arg, int zero, int lower)
  {
    *out = "_qrintf_chk_llx(" + *out + ", " + arg + ")";
  }
  void qrintf_d(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_d(" + *out + ", " + arg + ")";
  }
  void qrintf_hd(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_hd(" + *out + ", " + arg + ")";
  }
  void qrintf_ld(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_ld(" + *out + ", " + arg + ")";
  }
  void qrintf_lld(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_lld(" + *out + ", " + arg + ")";
  }
  void qrintf_u(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_u(" + *out + ", " + arg + ")";
  }
  void qrintf_hu(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_hu(" + *out + ", " + arg + ")";
  }
  void qrintf_llu(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_llu(" + *out + ", " + arg + ")";
  }
  void qrintf_lu(std::string *out, FormatArgument &fmt, std::string arg, int zero)
  {
    *out = "_qrintf_chk_lu(" + *out + ", " + arg + ")";
  }
  void qrintf_c(std::string *out, FormatArgument &fmt, std::string arg)
  {
    *out = "_qrintf_chk_c(" + *out + ", " + arg + ")";
  }
  void qrintf_s(std::string *out, FormatArgument &fmt, std::string arg)
  {
    *out = "_qrintf_chk_s(" + *out + ", " + arg + ")";
  }
  void qrintf_s_const(std::string *out, const char *str)
  {
    std::string len = std::to_string(strlen(str));
    *out = "_qrintf_chk_s_len(" + *out + ", " + str + ")";
  }
  void qrintf_string(std::string *out, FormatArgument &arg) {
    const char *text = arg.data_;
    int n = arg.length_;
    if (n == 1) {
      *out = "_qrintf_chk_c(" + *out + ", " + text[0] + ")";
    }
    else {
      std::string len = std::to_string(n);
      std::string str(text, n);
      *out = "_qrintf_chk_s_len(" + *out + ", \"" + str + "\", " + len + ")";
    }
  }
  void qrintf_s_precision(std::string *out, FormatArgument &fmt, std::string arg1, std::string arg2)
  {
    *out = "_qrintf_chk_s_len(" + *out + ", " + arg1 + ", " + arg2 + ")";
  }

  bool emit(std::string *out, std::vector<std::string> &args, int idx, FormatArgument &arg);
public:
  bool apply(StringRef Fmt,
      std::string buf,
      std::string buflen,
      std::vector<std::string> &args, std::string *out);
  bool check(StringRef Fmt);
};

bool SprintfOptimizer::emit(std::string *out, std::vector<std::string> &args, int idx, FormatArgument &arg)
{
  if (arg.Sharp()) {
    // not supported
    return false;
  }
  if (arg.Space()) {
    // not supported
    return false;
  }
  if (arg.Plus()) {
    // not supported
    return false;
  }
  if (arg.Precision()) {
    if (arg.precision_ != '*') {
      // not supported
      return false;
    }
  }

  if (arg.Hex()) {
    if (arg.Half())
      qrintf_hx(out, arg, args[idx], arg.Zero(), arg.LowerCase());
    else if (arg.Long())
      qrintf_lx(out, arg, args[idx], arg.Zero(), arg.LowerCase());
    else if (arg.LongLong())
      qrintf_llx(out, arg, args[idx], arg.Zero(), arg.LowerCase());
    else
      qrintf_x(out, arg, args[idx], arg.Zero(), arg.LowerCase());
    return true;
  }
  else if (arg.SInt()) {
    if (arg.Half())
      qrintf_hd(out, arg, args[idx], arg.Zero());
    else if (arg.Long())
      qrintf_ld(out, arg, args[idx], arg.Zero());
    else if (arg.LongLong())
      qrintf_lld(out, arg, args[idx], arg.Zero());
    else
      qrintf_d(out, arg, args[idx], arg.Zero());
    return true;
  }
  else if (arg.UInt()) {
    if (arg.Half())
      qrintf_hu(out, arg, args[idx], arg.Zero());
    else if (arg.Long())
      qrintf_lu(out, arg, args[idx], arg.Zero());
    else if (arg.LongLong())
      qrintf_llu(out, arg, args[idx], arg.Zero());
    else
      qrintf_u(out, arg, args[idx], arg.Zero());
    return true;
  }
  if (arg.Percent()) {
    qrintf_s_const(out, "%");
    return true;
  }
  if (arg.Pointer()) {
    qrintf_s_const(out, "0x");
    qrintf_lx(out, arg, args[idx], arg.Zero(), arg.LowerCase());
    return true;
  }
  if (arg.String()) {
    if (arg.Precision()) {
      qrintf_s_precision(out, arg, args[idx], args[idx + 1]);
    }
    else {
      qrintf_s(out, arg, args[idx]);
    }
    return true;
  }
  if (arg.Char()) {
    qrintf_c(out, arg, args[idx]);
    return true;
  }
  if (arg.LongDouble()) {
    // qrintf not supported
    return false;
  }
  if (arg.DoubleE()) {
    // qrintf not supported
    return false;
  }
  if (arg.DoubleF()) {
    // qrintf not supported
    return false;
  }
  if (arg.DoubleG()) {
    // qrintf not supported
    return false;
  }
  if (arg.DoubleA()) {
    // qrintf not supported
    return false;
  }
  return true;
}

static inline bool is_digit(char c)
{
  return '0' <= c && c <= '9';
}

static inline int is_flag(char c)
{
  switch (c) {
  case '+':
  case ' ':
  case '#':
  case '0':
    return true;
  }
  return false;
}

bool SprintfOptimizer::check(StringRef Fmt) {
  const char *fmt = Fmt.begin();
  char c;
  for (;;) {
    unsigned flag = 0;
    int precision = 0;
    const char *prev = fmt;
    /* skip until '%' */
    while ((c = *fmt++) != '\0') {
      if (c == '%')
        break;
    }
    if (c == '\0')
      break;
    if ((c = *fmt++) == '\0')
      break;
    /* parse format place holder */
    /* step 1. parse parameter
     * syntax:: parameter -> Digits '$'
     * */
    if (is_digit(c)) {
      char last = c;
      prev = fmt;
      while ((c = *fmt++) != '\0') {
        if (!is_digit(c))
          break;
      }
      if (c != '$') {
        // parse failed.
        fmt = prev;
        c = last;
      }
      if (c == '\0')
        break;
    }
    /* step 2. parse flag
     * syntax:: flag -> [+ #0]*
     */
    if (is_flag(c)) {
      do {
        switch (c) {
        case '+':
          return false;
        case ' ':
          return false;
        case '#':
          return false;
        case '0':
          flag |= ZERO;
          break;
        default:
          goto finish_parse_flag;
        }
      } while ((c = *fmt++) != '\0');
finish_parse_flag:
      if (c == '\0')
        break;
    }
    /* step 3. parse width
     * syntax:: width -> [0-9]+
     */
    if (is_digit(c)) {
      while ((c = *fmt++) != '\0') {
        if (!is_digit(c))
          break;
      }
      if (c == '\0')
        break;
    }
    /* step 4. parse precision
     * syntax:: precision -> '.' ('*' / [0-9]+)
     */
    if (c == '.') {
      precision = c;
      c = *fmt++;
      if (c == '*')
        flag |= PRECISION;
      else if (is_digit(c)) {
        precision = c - '0';
        flag |= PRECISION;
        while ((c = *fmt++) != '\0') {
          if (!is_digit(c))
            break;
        }
      }
      if (c != '\0')
        break;
    }
    /* 5. parse length and parse type
     * syntax:: length -> 'hh' / 'h' / 'll' / 'l' / 'L' / 'z' / 'j' / 't'
     * syntax:: type -> [diufFeEgGxXscpaAn%]
     */
    switch (c) {
    case 'h':
      /* parse h(h?)[diuxXc] */
      c = *fmt++;
      flag |= HALF;
      if (c == 'h') {
        c = *fmt++;
        flag |= HALF_HALF;
      }
      break;
    case 'l':
      /* parse l(l?)[diuxXc] */
      c = *fmt++;
      flag |= LONG;
      if (c == 'l') {
        c = *fmt++;
        flag |= LONG_LONG;
      }
      break;
    case 'p':
      flag |= PRINT_POINTER;
      break;
    case 'c':
      flag |= PRINT_CHAR;
      break;
    case 's':
      flag |= PRINT_STRING;
      break;
    case '%':
      flag |= PRINT_PERCENT;
      break;
    case 'L':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
    case 'a':
    case 'A':
      return false;
    case 'd':
    case 'i':
    case 'u':
    case 'x':
    case 'X':
      break;
    case 'n':
    default:
      // error : syntax-error or unsupported-formatter
      return false;
    }
  }
  return true;
}

bool SprintfOptimizer::apply(StringRef Fmt,
    std::string buf,
    std::string buflen,
    std::vector<std::string> &args, std::string *out) {
  const char *fmt = Fmt.begin();
  char c;
  int idx = 0;
  *out = "_qrintf_chk_init(" + buf + ", " + buflen + ")";
  for (;;) {
    unsigned flag = 0;
    int length = 0;
    int width = 0;
    int precision = 0;
    const char *prev = fmt;
    /* skip until '%' */
    while ((c = *fmt++) != '\0') {
      if (c == '%')
        break;
    }
    if (prev + 1 != fmt) {
      FormatArgument arg(fmt - prev - 1, prev);
      qrintf_string(out, arg);
    }
    if (c == '\0')
      break;
    if ((c = *fmt++) == '\0')
      break;
    /* parse format place holder */
    /* step 1. parse parameter
     * syntax:: parameter -> Digits '$'
     * */
    if (is_digit(c)) {
      char last = c;
      prev = fmt;
      idx = c - '0';
      while ((c = *fmt++) != '\0') {
        if (!is_digit(c))
          break;
        idx = idx * 10 + c - '0';
      }
      if (c != '$') {
        // parse failed.
        idx = 0;
        fmt = prev;
        c = last;
      }
      if (c == '\0')
        break;
    }
    /* step 2. parse flag
     * syntax:: flag -> [+ #0]*
     */
    if (is_flag(c)) {
      do {
        switch (c) {
        case '+':
          flag |= PLUS;
          break;
        case ' ':
          flag |= SPACE;
          break;
        case '#':
          flag |= SHARP;
          break;
        case '0':
          flag |= ZERO;
          break;
        default:
          goto finish_parse_flag;
        }
      } while ((c = *fmt++) != '\0');
finish_parse_flag:
      if (c == '\0')
        break;
    }
    /* step 3. parse width
     * syntax:: width -> [0-9]+
     */
    if (is_digit(c)) {
      width = c - '0';
      while ((c = *fmt++) != '\0') {
        if (!is_digit(c))
          break;
        width = width * 10 + c - '0';
      }
      if (c == '\0')
        break;
    }
    /* step 4. parse precision
     * syntax:: precision -> '.' ('*' / [0-9]+)
     */
    if (c == '.') {
      precision = c;
      c = *fmt++;
      if (c == '*')
        flag |= PRECISION;
      else if (is_digit(c)) {
        precision = c - '0';
        flag |= PRECISION;
        while ((c = *fmt++) != '\0') {
          if (!is_digit(c))
            break;
          idx = idx * 10 + c - '0';
        }
      }
      if (c != '\0')
        break;
    }
    /* 5. parse length and parse type
     * syntax:: length -> 'hh' / 'h' / 'll' / 'l' / 'L' / 'z' / 'j' / 't'
     * syntax:: type -> [diufFeEgGxXscpaAn%]
     */
    switch (c) {
    case 'h':
      /* parse h(h?)[diuxXc] */
      c = *fmt++;
      flag |= HALF;
      if (c == 'h') {
        c = *fmt++;
        flag |= HALF_HALF;
      }
      goto parse_type;
    case 'l':
      /* parse l(l?)[diuxXc] */
      c = *fmt++;
      flag |= LONG;
      if (c == 'l') {
        c = *fmt++;
        flag |= LONG_LONG;
      }
      goto parse_type;
    case 'p':
      flag |= PRINT_POINTER;
      break;
    case 'c':
      flag |= PRINT_CHAR;
      break;
    case 's':
      flag |= PRINT_STRING;
      break;
    case '%':
      flag |= PRINT_PERCENT;
      break;
    case 'L':
      flag |= LONG_DOUBLE;
      /* FALLTHROUGH */
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
    case 'a':
    case 'A':
    case 'd':
    case 'i':
    case 'u':
    case 'x':
    case 'X':
      goto parse_type;
    case 'n':
    default:
      // error : syntax-error or unsupported-formatter
      goto error;
    }
    goto emit_code;
parse_type:
    switch (c) {
    case 'd':
    case 'i':
      flag |= PRINT_INT;
      break;
    case 'u':
      flag |= PRINT_UINT;
      break;
    case 'x':
      flag |= LOWER_CASE;
      /* FALLTHROUGH */
    case 'X':
      flag |= PRINT_HEX;
      break;
    case 'e':
      flag |= LOWER_CASE;
      /* FALLTHROUGH */
    case 'E':
      flag |= PRINT_DOUBLE_E;
      break;
    case 'f':
      flag |= LOWER_CASE;
      /* FALLTHROUGH */
    case 'F':
      flag |= PRINT_DOUBLE_F;
      break;
    case 'g':
      flag |= LOWER_CASE;
      /* FALLTHROUGH */
    case 'G':
      flag |= PRINT_DOUBLE_G;
      break;
    case 'a':
      flag |= LOWER_CASE;
      /* FALLTHROUGH */
    case 'A':
      flag |= PRINT_DOUBLE_HEX;
      break;
    default:
      break;
    }
emit_code:
    FormatArgument arg(flag, length, width, precision);
    std::string v = args[idx];
    if (emit(out, args, idx++, arg)) {
      if (arg.String() && arg.Precision()) {
        idx++;
      }
    }
    else {
      // std::cout << "idx=" << idx << "str=" << v << "\n";
      return false;
    }
  }
  *out = "_qrintf_chk_finalize(" + *out + ")";
  return true;
error:
  return false;
}
