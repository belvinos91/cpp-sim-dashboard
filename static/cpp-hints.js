// cpp-hints.js - code suggestions for the editor while you type.
// Three sources: snippets (whole blocks such as a for loop), C++ and standard-library
// names, and words already in your file. Ctrl+Space shows suggestions at any time.
(function () {
  const KEYWORDS = [
    'auto', 'bool', 'break', 'case', 'catch', 'char', 'class', 'const', 'constexpr', 'continue',
    'default', 'delete', 'do', 'double', 'else', 'enum', 'false', 'float', 'for', 'if', 'inline',
    'int', 'long', 'namespace', 'new', 'nullptr', 'override', 'private', 'protected', 'public',
    'return', 'short', 'signed', 'size_t', 'sizeof', 'static', 'static_cast', 'struct', 'switch',
    'template', 'this', 'throw', 'true', 'try', 'typename', 'unsigned', 'using', 'virtual', 'void',
    'volatile', 'while'
  ];
  const STD = [
    'std::cout', 'std::cerr', 'std::cin', 'std::endl', 'std::string', 'std::to_string', 'std::stoi',
    'std::stod', 'std::vector', 'std::array', 'std::map', 'std::unordered_map', 'std::pair',
    'std::make_pair', 'std::tuple', 'std::size_t', 'std::ofstream', 'std::ifstream', 'std::getline',
    'std::stringstream', 'std::sqrt', 'std::exp', 'std::log', 'std::pow', 'std::fabs', 'std::abs',
    'std::sin', 'std::cos', 'std::floor', 'std::ceil', 'std::round', 'std::min', 'std::max',
    'std::sort', 'std::accumulate', 'std::fill', 'std::swap', 'std::mt19937',
    'std::random_device', 'std::uniform_real_distribution', 'std::uniform_int_distribution',
    'std::normal_distribution', 'std::exponential_distribution', 'std::binomial_distribution',
    'std::poisson_distribution', 'std::unique_ptr', 'std::make_unique', 'std::shared_ptr',
    'std::exception', 'std::runtime_error', 'std::numeric_limits', 'std::boolalpha', 'std::fixed',
    'std::setprecision'
  ];
  const HEADERS = [
    'iostream', 'fstream', 'sstream', 'string', 'vector', 'array', 'map', 'unordered_map', 'cmath',
    'random', 'algorithm', 'numeric', 'memory', 'iomanip', 'limits', 'stdexcept', 'chrono', 'tuple'
  ];
  // $0 marks where the cursor goes after the snippet is inserted
  const SNIPPETS = [
    ['main', 'int main() { ... }', '#include <iostream>\n\nint main() {\n    $0\n    return 0;\n}'],
    ['fori', 'for (int i = 0; i < n; ++i)', 'for (int i = 0; i < $0; ++i) {\n    \n}'],
    ['forr', 'for (auto& x : container)', 'for (auto& x : $0) {\n    \n}'],
    ['while', 'while (condition) { }', 'while ($0) {\n    \n}'],
    ['if', 'if (condition) { }', 'if ($0) {\n    \n}'],
    ['ifelse', 'if (...) { } else { }', 'if ($0) {\n    \n} else {\n    \n}'],
    ['func', 'double f(double x) { }', 'double $0(double x) {\n    return x;\n}'],
    ['struct', 'struct Name { };', 'struct $0 {\n    double a;\n};'],
    ['class', 'class Name { public: ... };', 'class $0 {\npublic:\n    \nprivate:\n    \n};'],
    ['try', 'try { } catch (...) { }', 'try {\n    $0\n} catch (const std::exception& e) {\n    std::cerr << e.what() << "\\n";\n}'],
    ['cout', 'std::cout << ... << "\\n";', 'std::cout << $0 << "\\n";'],
    ['vec', 'std::vector<double> x;', 'std::vector<double> $0;'],
    ['csv', 'write data.csv (plotted)', 'std::ofstream out("data.csv");\nout << "t,x\\n";\n$0'],
    ['rng', 'random generator with seed', 'std::mt19937 gen($0);'],
    ['normal', 'normal distribution N(0,1)', 'std::normal_distribution<double> N(0.0, 1.0);$0'],
    ['uniform', 'uniform distribution U(0,1)', 'std::uniform_real_distribution<double> U(0.0, 1.0);$0'],
    ['expdist', 'exponential distribution', 'std::exponential_distribution<double> wait($0);']
  ];

  function insertSnippet(cm, data, completion) {
    const indent = cm.getLine(data.from.line).match(/^\s*/)[0];
    const body = completion.body.split('\n').map((l, i) => (i ? indent + l : l)).join('\n');
    const at = body.indexOf('$0');
    const text = body.replace('$0', '');
    cm.replaceRange(text, data.from, data.to);
    const before = text.slice(0, at === -1 ? text.length : at).split('\n');
    const line = data.from.line + before.length - 1;
    const ch = (before.length === 1 ? data.from.ch : 0) + before[before.length - 1].length;
    cm.setCursor({ line, ch });
  }

  function cppHint(cm) {
    const cur = cm.getCursor();
    const lineText = cm.getLine(cur.line).slice(0, cur.ch);
    const word = (lineText.match(/[A-Za-z_][\w:]*$/) || [''])[0];
    const from = CodeMirror.Pos(cur.line, cur.ch - word.length), to = cur;
    const lower = word.toLowerCase();
    const list = [];

    if (/#include\s*<\w*$/.test(lineText)) {              // header names after #include <
      HEADERS.filter(h => h.startsWith(lower)).forEach(h => list.push({ text: h + '>', displayText: h }));
      return { list, from, to };
    }
    if (!word) return null;
    SNIPPETS.filter(([t]) => t.startsWith(lower)).forEach(([t, label, body]) =>
      list.push({ text: t, displayText: `${t}  ·  ${label}`, body, hint: insertSnippet, className: 'hint-snippet' }));
    const seen = new Set(list.map(x => x.text));
    const add = w => { if (w !== word && !seen.has(w) && w.toLowerCase().startsWith(lower)) { seen.add(w); list.push(w); } };
    STD.forEach(add);
    STD.forEach(w => { if (!lower.startsWith('std')) add(w.slice(5)); });  // 'vec' also offers 'vector'
    KEYWORDS.forEach(add);
    const docWords = new Set(cm.getValue().match(/[A-Za-z_]\w{2,}/g) || []);
    docWords.forEach(add);
    return list.length ? { list: list.slice(0, 40), from, to } : null;
  }

  window.setupCppHints = function (cm) {
    cm.setOption('hintOptions', { hint: cppHint, completeSingle: false });
    cm.on('inputRead', (cm, change) => {
      const ch = change.text[0];
      if (change.origin === '+input' && /^[\w<:]$/.test(ch) && !cm.state.completionActive) {
        const line = cm.getLine(change.from.line).slice(0, change.from.ch + 1);
        if (/[A-Za-z_][\w:]{1,}$/.test(line) || /#include\s*<$/.test(line)) cm.showHint();
      }
    });
  };
})();
