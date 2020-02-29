<TeXmacs|1.99.12>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package|giac|1.0>

    <\src-purpose>
      Markup for Giac sessions.
    </src-purpose>

    <src-copyright|2002--2004|Joris van der Hoeven>

    <\src-license>
      This software falls under the <hlink|GNU general public license,
      version 3 or later|$TEXMACS_PATH/LICENSE>. It comes WITHOUT ANY
      WARRANTY WHATSOEVER. You should have received a copy of the license
      which the software. If not, see <hlink|http://www.gnu.org/licenses/gpl-3.0.html|http://www.gnu.org/licenses/gpl-3.0.html>.
    </src-license>
  </src-title>>

  <assign|giac-output|<macro|body|<generic-output*|<arg|body>>>>

  \;

  <with|mode|math|<assign|derive|<macro|x|n|f|<frac|\<mathd\><rsup|<arg|n>>|\<mathd\><space|0.2spc><arg|x><rsup|<arg|n>>><space|0.2spc><arg|f>>>>

  \;

  <with|mode|math|<assign|derivative|<macro|f|x|n|<frac|\<mathd\><rsup|<arg|n>><space|0.2spc><arg|f>|\<mathd\><space|0.2spc><arg|x><rsup|<arg|n>>>>>>

  \;

  <with|mode|math|<assign|limit|<macro|x|a|f|lim<rsub|<arg|x>\<rightarrow\><arg|a>><space|0.2spc><arg|f>>>>

  \;

  <with|mode|math|<assign|llimit|<macro|x|a|f|lim<rsub|<arg|x>\<uparrow\><arg|a>><space|0.2spc><arg|f>>>>

  \;

  <with|mode|math|<assign|rlimit|<macro|x|a|f|lim<rsub|<arg|x>\<downarrow\><arg|a>><space|0.2spc><arg|f>>>>

  \;
</body>

<\initial>
  <\collection>
    <associate|preamble|true>
  </collection>
</initial>