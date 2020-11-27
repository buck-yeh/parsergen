# parsergen
- Parser/scanner generator suite able to generate LR1/GLR parser/scanner source code from a set of annotated production rules.
- The generated code will contain C++20 keywords, e.g. **constinit**
- In application point of view:
  - Sometimes you might reuse an existing scanner with another parser.
  - Sometimes all you need is just scanner. 
