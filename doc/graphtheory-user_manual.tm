<TeXmacs|1.99.7>

<style|<tuple|mmxmanual|framed-session|giac>>

<\body>
  <\hide-preamble>
    \;

    <assign|figure-text|<macro|<localize|Fig.>>>

    <assign|index-text|<macro|Command <localize|Index>>>

    <assign|prologue-text|<macro|Introduction>>

    <assign|unfolded-io|<\macro|prompt|in|out>
      <\surround||<right-flush>>
        <\input|<with|locus-color|preserve|<action|<arg|prompt>|mouse-fold|<arg|in>>>>
          <arg|in>
        </input>

        <\output>
          <arg|out>
        </output>
      </surround>
    </macro>>
  </hide-preamble>

  <\doc-data|<doc-title|Graph theory package for
  Giac/Xcas>|<doc-subtitle|Reference manual>|<\doc-date>
    October 2018
  </doc-date>||<doc-running-author|Luka Marohni¢>>
    \;
  <|doc-data>
    \;
  </doc-data>

  <\table-of-contents|toc>
    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|Introduction>
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-1><vspace|0.5fn>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|1.<space|2spc>Constructing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-2><vspace|0.5fn>

    1.1.<space|2spc>General graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-3>

    <with|par-left|1tab|1.1.1.<space|2spc>Undirected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-4>>

    <with|par-left|1tab|1.1.2.<space|2spc>Directed graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-6>>

    <with|par-left|1tab|1.1.3.<space|2spc>Examples
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-8>>

    <with|par-left|4tab|Creating vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-9><vspace|0.15fn>>

    <with|par-left|4tab|Creating edges and arcs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-10><vspace|0.15fn>>

    <with|par-left|4tab|Creating paths and trails
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-11><vspace|0.15fn>>

    <with|par-left|4tab|Specifying adjacency or weight matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-12><vspace|0.15fn>>

    <with|par-left|4tab|Creating special graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-13><vspace|0.15fn>>

    1.2.<space|2spc>Cycle and path graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-14>

    <with|par-left|1tab|1.2.1.<space|2spc>Cycle graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-15>>

    <with|par-left|1tab|1.2.2.<space|2spc>Path graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-17>>

    <with|par-left|1tab|1.2.3.<space|2spc>Trails of edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-19>>

    1.3.<space|2spc>Complete graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-22>

    <with|par-left|1tab|1.3.1.<space|2spc>Complete (multipartite) graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-23>>

    <with|par-left|1tab|1.3.2.<space|2spc>Complete trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-25>>

    1.4.<space|2spc>Sequence graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-28>

    <with|par-left|1tab|1.4.1.<space|2spc>Creating graphs from degree
    sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-29>>

    <with|par-left|1tab|1.4.2.<space|2spc>Validating graphic sequences
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-31>>

    1.5.<space|2spc>Intersection graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-33>

    <with|par-left|1tab|1.5.1.<space|2spc>Interval graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-34>>

    <with|par-left|1tab|1.5.2.<space|2spc>Kneser graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-36>>

    1.6.<space|2spc>Special graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-39>

    <with|par-left|1tab|1.6.1.<space|2spc>Hypercube graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-40>>

    <with|par-left|1tab|1.6.2.<space|2spc>Star graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-42>>

    <with|par-left|1tab|1.6.3.<space|2spc>Wheel graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-44>>

    <with|par-left|1tab|1.6.4.<space|2spc>Web graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-46>>

    <with|par-left|1tab|1.6.5.<space|2spc>Prism graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-48>>

    <with|par-left|1tab|1.6.6.<space|2spc>Antiprism graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-50>>

    <with|par-left|1tab|1.6.7.<space|2spc>Grid graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-52>>

    <with|par-left|1tab|1.6.8.<space|2spc>Sierpi«ski graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-55>>

    <with|par-left|1tab|1.6.9.<space|2spc>Generalized Petersen graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-57>>

    <with|par-left|1tab|1.6.10.<space|2spc><rigid|LCF> graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-59>>

    1.7.<space|2spc>Isomorphic copies of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-61>

    <with|par-left|1tab|1.7.1.<space|2spc>Creating isomorphic copies from
    permutations <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-62>>

    <with|par-left|1tab|1.7.2.<space|2spc>Permuting vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-64>>

    <with|par-left|1tab|1.7.3.<space|2spc>Relabeling vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-66>>

    1.8.<space|2spc>Subgraphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-68>

    <with|par-left|1tab|1.8.1.<space|2spc>Extracting subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-69>>

    <with|par-left|1tab|1.8.2.<space|2spc>Induced subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-71>>

    <with|par-left|1tab|1.8.3.<space|2spc>Underlying graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-73>>

    <with|par-left|1tab|1.8.4.<space|2spc>Fundamental cycles
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-75>>

    1.9.<space|2spc>Operations on graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-78>

    <with|par-left|1tab|1.9.1.<space|2spc>Graph complement
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-79>>

    <with|par-left|1tab|1.9.2.<space|2spc>Seidel switching
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-81>>

    <with|par-left|1tab|1.9.3.<space|2spc>Transposing graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-83>>

    <with|par-left|1tab|1.9.4.<space|2spc>Union of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-85>>

    <with|par-left|1tab|1.9.5.<space|2spc>Disjoint union of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-87>>

    <with|par-left|1tab|1.9.6.<space|2spc>Joining two graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-89>>

    <with|par-left|1tab|1.9.7.<space|2spc>Power graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-91>>

    <with|par-left|1tab|1.9.8.<space|2spc>Graph products
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-93>>

    <with|par-left|1tab|1.9.9.<space|2spc>Transitive closure graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-96>>

    <with|par-left|1tab|1.9.10.<space|2spc>Line graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-98>>

    <with|par-left|1tab|1.9.11.<space|2spc>Plane dual graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-100>>

    <with|par-left|1tab|1.9.12.<space|2spc>Truncating planar graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-103>>

    1.10.<space|2spc>Random graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-105>

    <with|par-left|1tab|1.10.1.<space|2spc>Random general graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-106>>

    <with|par-left|1tab|1.10.2.<space|2spc>Random bipartite graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-109>>

    <with|par-left|1tab|1.10.3.<space|2spc>Random trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-111>>

    <with|par-left|1tab|1.10.4.<space|2spc>Random planar graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-113>>

    <with|par-left|1tab|1.10.5.<space|2spc>Random graphs from a given degree
    sequence <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-115>>

    <with|par-left|1tab|1.10.6.<space|2spc>Random regular graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-117>>

    <with|par-left|1tab|1.10.7.<space|2spc>Random tournaments
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-119>>

    <with|par-left|1tab|1.10.8.<space|2spc>Random network graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-121>>

    <with|par-left|1tab|1.10.9.<space|2spc>Randomizing edge weights
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-123>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|2.<space|2spc>Modifying
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-125><vspace|0.5fn>

    2.1.<space|2spc>Promoting to directed and weighted graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-126>

    <with|par-left|1tab|2.1.1.<space|2spc>Converting edges to arcs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-127>>

    <with|par-left|1tab|2.1.2.<space|2spc>Assigning weight matrix to
    unweighted graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-129>>

    2.2.<space|2spc>Modifying vertices of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-131>

    <with|par-left|1tab|2.2.1.<space|2spc>Adding and removing vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-132>>

    2.3.<space|2spc>Modifying edges of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-135>

    <with|par-left|1tab|2.3.1.<space|2spc>Adding and removing edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-136>>

    <with|par-left|1tab|2.3.2.<space|2spc>Accessing and modifying edge
    weights <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-141>>

    <with|par-left|1tab|2.3.3.<space|2spc>Contracting edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-144>>

    <with|par-left|1tab|2.3.4.<space|2spc>Subdividing edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-146>>

    2.4.<space|2spc>Using attributes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-148>

    <with|par-left|1tab|2.4.1.<space|2spc>Graph attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-149>>

    <with|par-left|1tab|2.4.2.<space|2spc>Vertex attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-154>>

    <with|par-left|1tab|2.4.3.<space|2spc>Edge attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-159>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|3.<space|2spc>Import
    and export> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-164><vspace|0.5fn>

    3.1.<space|2spc>Importing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-165>

    <with|par-left|1tab|3.1.1.<space|2spc>Loading graphs from
    <with|font-family|ss|dot> files <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-166>>

    <with|par-left|1tab|3.1.2.<space|2spc>The <with|font-family|ss|dot> file
    format overview <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-168>>

    3.2.<space|2spc>Exporting graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-169>

    <with|par-left|1tab|3.2.1.<space|2spc>Saving graphs in
    <with|font-family|ss|dot> format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-170>>

    <with|par-left|1tab|3.2.2.<space|2spc>Saving graph drawings in
    L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|0|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
    format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-172>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|4.<space|2spc>Graph
    properties> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-174><vspace|0.5fn>

    4.1.<space|2spc>Basic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-175>

    <with|par-left|1tab|4.1.1.<space|2spc>Determining the type of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-176>>

    <with|par-left|1tab|4.1.2.<space|2spc>Listing vertices and edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-179>>

    <with|par-left|1tab|4.1.3.<space|2spc>Equality of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-185>>

    <with|par-left|1tab|4.1.4.<space|2spc>Vertex degrees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-187>>

    <with|par-left|1tab|4.1.5.<space|2spc>Regular graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-194>>

    <with|par-left|1tab|4.1.6.<space|2spc>Strongly regular graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-196>>

    <with|par-left|1tab|4.1.7.<space|2spc>Vertex adjacency
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-198>>

    <with|par-left|1tab|4.1.8.<space|2spc>Tournament graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-204>>

    <with|par-left|1tab|4.1.9.<space|2spc>Bipartite graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-206>>

    <with|par-left|1tab|4.1.10.<space|2spc>Edge incidence
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-208>>

    4.2.<space|2spc>Algebraic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-210>

    <with|par-left|1tab|4.2.1.<space|2spc>Adjacency matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-211>>

    <with|par-left|1tab|4.2.2.<space|2spc>Laplacian matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-213>>

    <with|par-left|1tab|4.2.3.<space|2spc>Incidence matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-215>>

    <with|par-left|1tab|4.2.4.<space|2spc>Weight matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-217>>

    <with|par-left|1tab|4.2.5.<space|2spc>Characteristic polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-219>>

    <with|par-left|1tab|4.2.6.<space|2spc>Graph spectrum
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-221>>

    <with|par-left|1tab|4.2.7.<space|2spc>Seidel spectrum
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-223>>

    <with|par-left|1tab|4.2.8.<space|2spc>Integer graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-225>>

    4.3.<space|2spc>Graph isomorphism <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-227>

    <with|par-left|1tab|4.3.1.<space|2spc>Isomorphic graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-228>>

    <with|par-left|1tab|4.3.2.<space|2spc>Canonical labeling
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-230>>

    <with|par-left|1tab|4.3.3.<space|2spc>Graph automorphisms
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-232>>

    4.4.<space|2spc>Graph polynomials <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-234>

    <with|par-left|1tab|4.4.1.<space|2spc>Tutte polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-235>>

    <with|par-left|1tab|4.4.2.<space|2spc>Chromatic polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-237>>

    <with|par-left|1tab|4.4.3.<space|2spc>Flow polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-239>>

    <with|par-left|1tab|4.4.4.<space|2spc>Reliability polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-241>>

    4.5.<space|2spc>Connectivity <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-243>

    <with|par-left|1tab|4.5.1.<space|2spc>Connected, biconnected and
    triconnected graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-244>>

    <with|par-left|1tab|4.5.2.<space|2spc>Connected and biconnected
    components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-248>>

    <with|par-left|1tab|4.5.3.<space|2spc>Vertex connectivity
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-251>>

    <with|par-left|1tab|4.5.4.<space|2spc>Graph rank
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-253>>

    <with|par-left|1tab|4.5.5.<space|2spc>Articulation points
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-255>>

    <with|par-left|1tab|4.5.6.<space|2spc>Strongly connected components
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-257>>

    <with|par-left|1tab|4.5.7.<space|2spc>Edge connectivity
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-260>>

    <with|par-left|1tab|4.5.8.<space|2spc>Edge cuts
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-262>>

    <with|par-left|1tab|4.5.9.<space|2spc>Two-edge-connected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-264>>

    4.6.<space|2spc>Trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-267>

    <with|par-left|1tab|4.6.1.<space|2spc>Tree graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-268>>

    <with|par-left|1tab|4.6.2.<space|2spc>Forest graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-270>>

    <with|par-left|1tab|4.6.3.<space|2spc>Height of a tree
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-272>>

    <with|par-left|1tab|4.6.4.<space|2spc>Lowest common ancestor of a pair of
    nodes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-274>>

    <with|par-left|1tab|4.6.5.<space|2spc>Arborescence graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-276>>

    4.7.<space|2spc>Networks <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-278>

    <with|par-left|1tab|4.7.1.<space|2spc>Network graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-279>>

    <with|par-left|1tab|4.7.2.<space|2spc>Maximum flow
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-281>>

    <with|par-left|1tab|4.7.3.<space|2spc>Minimum cut
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-283>>

    4.8.<space|2spc>Distance in graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-285>

    <with|par-left|1tab|4.8.1.<space|2spc>Vertex distance
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-286>>

    <with|par-left|1tab|4.8.2.<space|2spc>All-pairs vertex distance
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-288>>

    <with|par-left|1tab|4.8.3.<space|2spc>Diameter
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-290>>

    <with|par-left|1tab|4.8.4.<space|2spc>Girth
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-291>>

    4.9.<space|2spc>Acyclic graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-294>

    <with|par-left|1tab|4.9.1.<space|2spc>Acyclic graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-295>>

    <with|par-left|1tab|4.9.2.<space|2spc>Topological sorting
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-297>>

    <with|par-left|1tab|4.9.3.<space|2spc><with|font-family|ss|st> ordering
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-300>>

    4.10.<space|2spc>Matching in graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-302>

    <with|par-left|1tab|4.10.1.<space|2spc>Maximum matching
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-303>>

    <with|par-left|1tab|4.10.2.<space|2spc>Maximum matching in bipartite
    graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-305>>

    4.11.<space|2spc>Cliques <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-307>

    <with|par-left|1tab|4.11.1.<space|2spc>Clique graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-308>>

    <with|par-left|1tab|4.11.2.<space|2spc>Maximal cliques
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-310>>

    <with|par-left|1tab|4.11.3.<space|2spc>Maximum clique
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-312>>

    <with|par-left|1tab|4.11.4.<space|2spc>Minimum clique cover
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-315>>

    <with|par-left|1tab|4.11.5.<space|2spc>Clique cover number
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-317>>

    4.12.<space|2spc>Triangles in graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-319>

    <with|par-left|1tab|4.12.1.<space|2spc>Counting triangles
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-320>>

    <with|par-left|1tab|4.12.2.<space|2spc>Clustering coefficient
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-322>>

    <with|par-left|1tab|4.12.3.<space|2spc>Network transitivity
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-324>>

    4.13.<space|2spc>Vertex coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-326>

    <with|par-left|1tab|4.13.1.<space|2spc>Greedy vertex coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-327>>

    <with|par-left|1tab|4.13.2.<space|2spc>Minimal vertex coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-330>>

    <with|par-left|1tab|4.13.3.<space|2spc>Chromatic number
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-332>>

    <with|par-left|1tab|4.13.4.<space|2spc>Mycielski graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-334>>

    <with|par-left|1tab|4.13.5.<space|2spc><with|mode|math|k>-coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-336>>

    4.14.<space|2spc>Edge coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-338>

    <with|par-left|1tab|4.14.1.<space|2spc>Minimal edge coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-339>>

    <with|par-left|1tab|4.14.2.<space|2spc>Chromatic index
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-341>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|5.<space|2spc>Traversing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-343><vspace|0.5fn>

    5.1.<space|2spc>Walks and tours <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-344>

    <with|par-left|1tab|5.1.1.<space|2spc>Eulerian graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-345>>

    <with|par-left|1tab|5.1.2.<space|2spc>Hamiltonian graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-347>>

    5.2.<space|2spc>Optimal routing <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-349>

    <with|par-left|1tab|5.2.1.<space|2spc>Shortest unweighted paths
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-350>>

    <with|par-left|1tab|5.2.2.<space|2spc>Cheapest weighted paths
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-352>>

    <with|par-left|1tab|5.2.3.<space|2spc>Traveling salesman problem
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-355>>

    5.3.<space|2spc>Spanning trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-357>

    <with|par-left|1tab|5.3.1.<space|2spc>Construction of spanning trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-358>>

    <with|par-left|1tab|5.3.2.<space|2spc>Minimal spanning tree
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-360>>

    <with|par-left|1tab|5.3.3.<space|2spc>Counting the spanning trees in a
    graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-362>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|6.<space|2spc>Visualizing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-364><vspace|0.5fn>

    6.1.<space|2spc>Drawing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-365>

    <with|par-left|1tab|6.1.1.<space|2spc>Overview
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-367>>

    <with|par-left|1tab|6.1.2.<space|2spc>Drawing disconnected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-368>>

    <with|par-left|1tab|6.1.3.<space|2spc>Spring method
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-369>>

    <with|par-left|1tab|6.1.4.<space|2spc>Drawing trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-370>>

    <with|par-left|1tab|6.1.5.<space|2spc>Drawing planar graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-371>>

    <with|par-left|1tab|6.1.6.<space|2spc>Circular graph drawings
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-375>>

    6.2.<space|2spc>Vertex positions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-376>

    <with|par-left|1tab|6.2.1.<space|2spc>Setting vertex positions
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-377>>

    <with|par-left|1tab|6.2.2.<space|2spc>Generating vertex positions
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-379>>

    6.3.<space|2spc>Highlighting parts of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-380>

    <with|par-left|1tab|6.3.1.<space|2spc>Highlighting vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-381>>

    <with|par-left|1tab|6.3.2.<space|2spc>Highlighting edges and trails
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-383>>

    <with|par-left|1tab|6.3.3.<space|2spc>Highlighting subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-386>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|Bibliography>
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-388><vspace|0.5fn>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|Command
    Index> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-389><vspace|0.5fn>
  </table-of-contents>

  <prologue>

  This document<\footnote>
    This manual was written in <hlink|<rigid|<abbr|GNU>
    <TeXmacs>>|http://www.texmacs.org/tmweb/home/welcome.en.html>, a
    scientific document editing platform. All examples were entered as
    interactive <samp|Giac> sessions.
  </footnote> contains an overview of the library of graph theory commands
  built in <samp|Giac> computation kernel and supported within the
  <samp|Xcas> <abbr|GUI>. The library provides an effective and free
  replacement for the <abbr|GraphTheory> package in <samp|Maple> with a high
  level of syntax compatibility (although there are some minor differences).

  For each command, the calling syntax is presented along with the detailed
  description of its functionality and several examples. The square brackets
  <verbatim|[> and <verbatim|]> in the calling syntax indicate that the
  respective argument should be a list of particular elements or that its
  inclusion is optional. The character <verbatim|\|> stands for <em|or>.

  The algorithms in this library are implemented according to relevant
  scientific publications. Although the development focus was on simplicity,
  the algorithms are reasonably fast. For some more difficult tasks, such as
  for solving traveling salesman problem, finding graph colorings and graph
  isomorphism, freely available third party libraries are used, in particular
  <abbr|GNU> Linear Programming Kit (<samp|GLPK>) and <samp|nauty>. These
  libraries, included in <samp|Giac/Xcas> by default, are optional during the
  compilation; most commands have no dependencies save <samp|Giac> itself.

  This library was written and documented by Luka Marohni¢<\footnote>
    Email: <verbatim|luka.marohnic@tvz.hr>
  </footnote>. The author would like to thank Bernard Parisse, the
  <samp|Giac/Xcas> project leader, for integrating the package and Jose Capco
  for suggesting <samp|nauty> integration.

  <chapter|Constructing graphs>

  <section|General graphs>

  The commands <verbatim|graph> and <verbatim|digraph> are used for
  constructing general <hlink|graphs|http://www.encyclopediaofmath.org/index.php?title=Graph>.

  <subsection|Undirected graphs><label|graph>

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|11|3|3|cell-valign|c>|<cwith|1|11|3|3|cell-lsep|1em>|<cwith|6|6|3|3|cell-row-span|2>|<cwith|6|6|3|3|cell-col-span|1>|<cwith|1|11|2|3|cell-background|>|<table|<row|<\cell>
    Syntax:
  </cell>|<\cell>
    <verbatim|graph(n\|V,[opts])>
  </cell>|<\cell>
    <very-small|graph with <math|n> vertices or vertex set <math|V> and no
    edges>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(V,E,[opts])>
  </cell>|<\cell>
    <very-small|graph <math|<around*|(|V,E|)>>>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(E,[opts])>
  </cell>|<\cell>
    <very-small|graph with edge set <math|E>, vertices implied>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(V,T,[opts])>
  </cell>|<\cell>
    <very-small|graph <math|<around*|(|V,E|)>> where <math|E> is the edge set
    of trail <math|T>>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(T,[opts])>
  </cell>|<\cell>
    <very-small|graph with edges from trail <math|T>, vertices implied>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(V,T1,T2,T3,..,Tk,[opts])>
  </cell>|<\cell>
    <very-small|graph with edge set consisting of edges on the given trails>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(T1,T2,T3,..,Tk,[opts])>
  </cell>|<\cell>
    \;
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(A,[opts])>
  </cell>|<\cell>
    <very-small|graph with adjacency or weight matrix <math|A>, vertices
    implied>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(V,E,A,[opts])>
  </cell>|<\cell>
    <very-small|weighted graph <math|<around*|(|V,E|)>> with weight matrix
    <math|A>>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(V,Perm,[opts])>
  </cell>|<\cell>
    <very-small|digraph with a single cycle as a permutation of vertices
    <math|V>>
  </cell>>|<row|<\cell>
    \;
  </cell>|<\cell>
    <verbatim|graph(Str)>
  </cell>|<\cell>
    <very-small|special graph>
  </cell>>>>>

  The command<index|<verbatim|graph>> <verbatim|graph> takes between one and
  three main arguments, each of them being one of the following structural
  elements of the resulting graph <math|G<around*|(|V,E|)>>. Throughout this
  manual, an edge <math|e\<in\>E> with endpoints <math|u,v\<in\>V> is denoted
  by <math|e=u v>. Note that the order of the endpoints does not matter if
  <math|G> is undirected; hence <math|u v=v u>. If <math|G> is directed,
  <math|u v> and <math|v u> are treated as separate edges.

  <\itemize-minus>
    <item>number <math|n> or list of vertices <math|V> (a vertex may be any
    atomic object, such as an integer, a symbol or a string); it must be the
    first argument if used,

    <item>set of edges <math|E> (each edge is represented by the list of its
    endpoints), a permutation, a trail of edges or a sequence of trails; it
    can be either the first or the second argument if used,

    <item>trail <math|T> or sequence of trails
    <math|T<rsub|1>,T<rsub|2>,\<ldots\>,T<rsub|k>>,

    <item>permutation <verbatim|Perm> of vertices,

    <item>adjacency or weight matrix <math|A>,

    <item>string <verbatim|Str>, representing a special graph.
  </itemize-minus>

  Optionally, the following options may be appended to the sequence of
  arguments:

  <\itemize-minus>
    <item><verbatim|directed = true> or <verbatim|false>,

    <item><verbatim|weighted = true> or <verbatim|false>,

    <item><verbatim|color = >an integer or a list of integers representing
    color(s) of the vertices,

    <item><verbatim|coordinates = >a list of vertex <abbr|2D> or <abbr|3D>
    coordinates.
  </itemize-minus>

  The <with|font-family|tt|graph> command may also be called by passing a
  string <verbatim|Str>, representing the name of a special graph, as its
  only argument. In that case the corresponding graph will be constructed and
  returned. The supported graphs are listed below.

  <center|<tabular|<tformat|<cwith|1|1|4|5|cell-tborder|0ln>|<cwith|1|1|4|5|cell-bborder|1ln>|<cwith|1|1|4|4|cell-lborder|0ln>|<cwith|1|1|3|3|cell-rborder|0ln>|<cwith|1|1|5|5|cell-rborder|0ln>|<cwith|1|1|1|2|cell-tborder|0ln>|<cwith|1|1|1|1|cell-lborder|0ln>|<cwith|1|1|2|2|cell-rborder|0ln>|<cwith|1|1|3|3|cell-lborder|0ln>|<cwith|1|1|3|3|cell-width|1em>|<cwith|1|1|3|3|cell-hmode|exact>|<cwith|7|7|3|3|cell-rborder|0ln>|<cwith|4|4|1|2|cell-bborder|0ln>|<cwith|6|6|1|2|cell-tborder|0ln>|<cwith|4|4|1|1|cell-lborder|0ln>|<cwith|4|4|2|2|cell-rborder|0ln>|<cwith|4|4|3|3|cell-lborder|0ln>|<cwith|2|2|1|2|cell-tborder|1ln>|<cwith|1|1|1|2|cell-bborder|1ln>|<cwith|2|2|1|2|cell-bborder|0ln>|<cwith|4|4|1|2|cell-tborder|0ln>|<cwith|2|2|1|1|cell-lborder|0ln>|<cwith|2|2|2|2|cell-rborder|0ln>|<cwith|2|2|3|3|cell-lborder|0ln>|<cwith|6|6|4|5|cell-tborder|0ln>|<cwith|5|5|4|5|cell-bborder|0ln>|<cwith|6|6|4|5|cell-bborder|0ln>|<cwith|8|8|4|5|cell-tborder|0ln>|<cwith|6|6|4|4|cell-lborder|0ln>|<cwith|6|6|5|5|cell-rborder|0ln>|<table|<row|<cell|<strong|special
  graph>>|<cell|<strong|name in> <samp|Giac>>|<cell|>|<cell|<strong|special
  graph>>|<cell|<strong|name in> <samp|Giac>>>|<row|<cell|<hlink|<rigid|Balaban
  10-cage>|https://en.wikipedia.org/wiki/Balaban_10-cage>>|<cell|<verbatim|balaban10>>|<cell|>|<cell|<hlink|<rigid|Grötzsch
  graph>|https://en.wikipedia.org/wiki/Gr%C3%B6tzsch_graph>>|<cell|<with|font-family|tt|grotzsch>>>|<row|<cell|<hlink|<rigid|Balaban
  11-cage>|https://en.wikipedia.org/wiki/Balaban_11-cage>>|<cell|<verbatim|balaban11>>|<cell|>|<cell|<hlink|<rigid|Harries
  graph>|https://en.wikipedia.org/wiki/Harries_graph>>|<cell|<with|font-family|tt|harries>>>|<row|<cell|<hlink|<rigid|Bidiakis
  cube>|https://en.wikipedia.org/wiki/Bidiakis_cube>>|<cell|<verbatim|bidiakis>>|<cell|>|<cell|<hlink|<rigid|Harries\UWong
  graph>|https://en.wikipedia.org/wiki/Harries%E2%80%93Wong_graph>>|<cell|<with|font-family|tt|harries-wong>>>|<row|<cell|<hlink|<rigid|Biggs-Smith
  graph>|https://en.wikipedia.org/wiki/Biggs%E2%80%93Smith_graph>>|<cell|<verbatim|biggs-smith>>|<cell|>|<cell|<hlink|<rigid|Heawood
  graph>|https://en.wikipedia.org/wiki/Heawood_graph>>|<cell|<with|font-family|tt|heawood>>>|<row|<cell|2<rsup|nd>
  <hlink|<rigid|Blanu²a snark>|https://en.wikipedia.org/wiki/Blanu%C5%A1a_snarks>>|<cell|<verbatim|blanusa>>|<cell|>|<cell|<hlink|<rigid|Herschel
  graph>|https://en.wikipedia.org/wiki/Herschel_graph>>|<cell|<with|font-family|tt|herschel>>>|<row|<cell|<hlink|<rigid|Bull
  graph>|https://en.wikipedia.org/wiki/Bull_graph>>|<cell|<verbatim|bull>>|<cell|>|<cell|<hlink|<rigid|Hoffman
  graph>|https://en.wikipedia.org/wiki/Hoffman_graph>>|<cell|<verbatim|hoffman>>>|<row|<cell|<hlink|<rigid|Butterfly
  graph>|https://en.wikipedia.org/wiki/Butterfly_graph>>|<cell|<verbatim|butterfly>>|<cell|>|<cell|<hlink|<rigid|Icosahedral
  graph>|https://en.wikipedia.org/wiki/Regular_icosahedron#Icosahedral_graph>>|<cell|<with|font-family|tt|icosahedron>>>|<row|<cell|<hlink|<rigid|Clebsch
  graph>|https://en.wikipedia.org/wiki/Clebsch_graph>>|<cell|<with|font-family|tt|clebsch>>|<cell|>|<cell|<hlink|<rigid|Levi
  graph>|https://en.wikipedia.org/wiki/Levi_graph> (Tutte
  8-cage)>|<cell|<with|font-family|tt|levi>>>|<row|<cell|<hlink|<rigid|Chvátal
  graph>|https://en.wikipedia.org/wiki/Chv%C3%A1tal_graph>>|<cell|<verbatim|chvatal>>|<cell|>|<cell|<hlink|<rigid|Ljubljana
  graph>|https://en.wikipedia.org/wiki/Ljubljana_graph>>|<cell|<with|font-family|tt|ljubljana>>>|<row|<cell|<hlink|<rigid|Coxeter
  graph>|https://en.wikipedia.org/wiki/Coxeter_graph>>|<cell|<with|font-family|tt|coxeter>>|<cell|>|<cell|<hlink|<rigid|McGee
  graph>|https://en.wikipedia.org/wiki/McGee_graph>>|<cell|<with|font-family|tt|mcgee>>>|<row|<cell|<hlink|<rigid|Desargues
  graph>|https://en.wikipedia.org/wiki/Desargues_graph>>|<cell|<with|font-family|tt|desargues>>|<cell|>|<cell|<hlink|<rigid|Moser
  spindle>|https://en.wikipedia.org/wiki/Moser_spindle>>|<cell|<verbatim|moser>>>|<row|<cell|<hlink|<rigid|Diamond
  graph>|https://en.wikipedia.org/wiki/Diamond_graph>>|<cell|<verbatim|diamond>>|<cell|>|<cell|<hlink|<rigid|Möbius\UKantor
  graph>|https://en.wikipedia.org/wiki/M%C3%B6bius%E2%80%93Kantor_graph>>|<cell|<verbatim|mobius-kantor>>>|<row|<cell|<hlink|<rigid|Dodecahedral
  graph>|https://en.wikipedia.org/wiki/Regular_dodecahedron#Dodecahedral_graph>>|<cell|<with|font-family|tt|dodecahedron>>|<cell|>|<cell|<hlink|<rigid|Nauru
  graph>|https://en.wikipedia.org/wiki/Nauru_graph>>|<cell|<with|font-family|tt|nauru>>>|<row|<cell|<hlink|<rigid|Dürer
  graph>|https://en.wikipedia.org/wiki/D%C3%BCrer_graph>>|<cell|<with|font-family|tt|durer>>|<cell|>|<cell|<hlink|<rigid|Octahedral
  graph>|https://en.wikipedia.org/wiki/Platonic_graph>>|<cell|<with|font-family|tt|octahedron>>>|<row|<cell|<hlink|<rigid|Dyck
  graph>|https://en.wikipedia.org/wiki/Dyck_graph>>|<cell|<with|font-family|tt|dyck>>|<cell|>|<cell|<hlink|<rigid|Pappus
  graph>|https://en.wikipedia.org/wiki/Pappus_graph>>|<cell|<with|font-family|tt|pappus>>>|<row|<cell|<hlink|<rigid|Errera
  graph>|https://en.wikipedia.org/wiki/Errera_graph>>|<cell|<verbatim|errera>>|<cell|>|<cell|<hlink|<rigid|Petersen
  graph>|https://en.wikipedia.org/wiki/Petersen_graph>>|<cell|<with|font-family|tt|petersen>>>|<row|<cell|<hlink|<rigid|F26A
  graph>|https://en.wikipedia.org/wiki/F26A_graph>>|<cell|<verbatim|f26a>>|<cell|>|<cell|<hlink|<rigid|Poussin
  graph>|https://en.wikipedia.org/wiki/Poussin_graph>>|<cell|<verbatim|poussin>>>|<row|<cell|<hlink|<rigid|Folkman
  graph>|https://en.wikipedia.org/wiki/Folkman_graph>>|<cell|<verbatim|folkman>>|<cell|>|<cell|<hlink|<rigid|Robertson
  graph>|https://en.wikipedia.org/wiki/Robertson_graph>>|<cell|<with|font-family|tt|robertson>>>|<row|<cell|<hlink|<rigid|Foster
  graph>|https://en.wikipedia.org/wiki/Foster_graph>>|<cell|<verbatim|foster>>|<cell|>|<cell|<hlink|<rigid|Truncated
  icosahedral graph>|https://en.wikipedia.org/wiki/Truncated_icosahedron#Truncated_icosahedral_graph>>|<cell|<verbatim|soccerball>>>|<row|<cell|<hlink|<rigid|Franklin
  graph>|https://en.wikipedia.org/wiki/Franklin_graph>>|<cell|<verbatim|franklin>>|<cell|>|<cell|<hlink|<rigid|Shrikhande
  graph>|https://en.wikipedia.org/wiki/Shrikhande_graph>>|<cell|<with|font-family|tt|shrikhande>>>|<row|<cell|<hlink|<rigid|Frucht
  graph>|https://en.wikipedia.org/wiki/Frucht_graph>>|<cell|<verbatim|frucht>>|<cell|>|<cell|<hlink|<rigid|Tetrahedral
  graph>|https://en.wikipedia.org/wiki/Tetrahedron#Tetrahedral_graph>>|<cell|<with|font-family|tt|tehtrahedron>>>|<row|<cell|<hlink|<rigid|Goldner-Harary
  graph>|https://en.wikipedia.org/wiki/Goldner%E2%80%93Harary_graph>>|<cell|<verbatim|goldner-harary>>|<cell|>|<cell|<hlink|<rigid|Tietze
  graph>|https://en.wikipedia.org/wiki/Tietze_graph>>|<cell|<verbatim|tietze>>>|<row|<cell|<hlink|<rigid|Golomb
  graph>|https://en.wikipedia.org/wiki/Golomb_graph>>|<cell|<verbatim|golomb>>|<cell|>|<cell|<hlink|<rigid|Tutte
  graph>|https://en.wikipedia.org/wiki/Tutte_graph>>|<cell|<verbatim|tutte>>>|<row|<cell|<hlink|<rigid|Gray
  graph>|https://en.wikipedia.org/wiki/Gray_graph>>|<cell|<verbatim|gray>>|<cell|>|<cell|<hlink|<rigid|Tutte
  12-cage>|https://en.wikipedia.org/wiki/Tutte_12-cage>>|<cell|<verbatim|tutte12>>>|<row|<cell|<hlink|<rigid|Grinberg
  graph>|https://en.wikipedia.org/wiki/Grinberg%27s_theorem>>|<cell|<with|font-family|tt|grinberg>>|<cell|>|<cell|<hlink|<rigid|Wagner
  graph>|https://en.wikipedia.org/wiki/Wagner_graph>>|<cell|<verbatim|wagner>>>>>>>

  <subsection|Directed graphs><label|digraph>

  The <with|font-family|tt|digraph> command<index|<verbatim|digraph>> is used
  for creating <hlink|<rigid|directed graphs>|http://www.encyclopediaofmath.org/index.php?title=Graph,_oriented>,
  although it is also possible with the <verbatim|<hlink|graph|#graph>>
  command by specifying the option <with|font-family|tt|directed=true>.
  Actually, calling <with|font-family|tt|digraph> is the same as calling
  <with|font-family|tt|graph> with that option appended to the sequence of
  arguments. However, creating special graphs is not supported by
  <with|font-family|tt|digraph> since they are all undirected.

  Edges in directed graphs are usually called <strong|arcs>.

  <subsection|Examples>

  <paragraph|Creating vertices>A graph consisting only of vertices and no
  edges can be created simply by providing the number of vertices or the list
  of vertex labels.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 0 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([a,b,c])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 0 edges>
      </equation*>
    </unfolded-io>
  </session>

  The commands that return graphs often need to generate vertex labels. In
  these cases ordinal integers are used, which are 0-based in <samp|Xcas>
  mode and 1-based in <samp|Maple> mode. Examples throughout this manual are
  made by using the default, <samp|Xcas> mode.\ 

  <paragraph|Creating edges and arcs>Edges/arcs must be specified inside a
  set so that it can be distinguished from a (adjacency or weight) matrix. If
  only a set of edges/arcs is specified, the vertices needed to establish
  these will be created automatically. Note that, when constructing a
  directed graph, the order of the vertices in an arc matters; in undirected
  graphs it is not meaningful.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph(%{[a,b],[b,c],[a,c]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  Edge weights may also be specified.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph(%{[[a,b],2],[[b,c],2.3],[[c,a],3/2]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  If the graph contains isolated vertices (not connected to any other vertex)
  or a particular order of vertices is desired, the list of vertices has to
  be specified first.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([d,b,c,a],%{[a,b],[b,c],[a,c]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <paragraph|Creating paths and trails>A directed graph can also be created
  from a list of <math|n> vertices and a permutation of order <math|n>. The
  resulting graph consists of a single directed cycle with the vertices
  ordered according to the permutation.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph([a,b,c,d],[1,2,3,0])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/dg1.eps|35%|||>>

  Alternatively, one may specify edges as a <verbatim|<hlink|trail|#trail>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      <with|font-family|tt|digraph([a,b,c,d],trail(b,c,d,a))>
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 3 arcs>
      </equation*>
    </unfolded-io>
  </session>

  Using trails is also possible when creating undirected graphs. Also, some
  vertices in a trail may be repeated, which is not allowed in a path.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph([a,b,c,d],trail(b,c,d,a,c))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|a>|<cell|c>>|<row|<cell|a>|<cell|d>>|<row|<cell|b>|<cell|c>>|<row|<cell|c>|<cell|d>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  It is possible to specify several trails in a sequence, which is useful
  when designing more complex graphs.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,2),trail(3,5,6,7,5,4))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/trails.eps|35%|||>>

  <paragraph|Specifying adjacency or weight matrix>A graph can be created
  from a single square matrix <math|A=<around|[|a<rsub|i*j>|]><rsub|n>> of
  order <math|n>. If it contains only ones and zeros and has zeros on its
  diagonal, it is assumed to be the adjacency matrix for the desired graph.
  Otherwise, if an element outside the set <math|<around|{|0,1|}>> is
  encountered, it is assumed that the matrix of edge weights is passed as
  input, causing the resulting graph to be weighted accordingly. In each
  case, exactly <math|n> vertices will be created and <math|i>-th and
  <math|j>-th vertex will be connected iff <math|a<rsub|i*j>\<neq\>0>. If the
  matrix is symmetric, the resulting graph will be undirected, otherwise it
  will be directed.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph([[0,1,1,0],[1,0,0,1],[1,0,0,0],[0,1,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|2>>|<row|<cell|1>|<cell|3>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph([[0,1.0,2.3,0],[4,0,0,3.1],[0,0,0,0],[0,0,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 4 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G,weights)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|<around|[|0,1|]>,1.0|]>,<around|[|<around|[|0,2|]>,2.3|]>,<around|[|<around|[|1,0|]>,4|]>,<around|[|<around|[|1,3|]>,3.1|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  List of vertex labels can be specified alongside a matrix.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([a,b,c,d],[[0,1,1,0],[1,0,0,1],[1,0,0,0],[0,1,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  When creating a weighted graph, one can first specify the list of <math|n>
  vertices and the set of edges, followed by a square matrix <math|A> of
  order <math|n>. Then for every edge <math|<around|{|i,j|}>> or arc
  <math|<around|(|i,j|)>> the element <math|a<rsub|i*j>> of <math|A> is
  assigned as its weight. Other elements of <math|A> are ignored.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph([a,b,c],%{[a,b],[b,c],[a,c]%},[[0,1,2],[3,0,4],[5,6,0]])
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G,weights)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|<around|[|a,b|]>,1|]>,<around|[|<around|[|a,c|]>,2|]>,<around|[|<around|[|b,c|]>,4|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <paragraph|Creating special graphs>When a special graph is desired, one
  just needs to pass its name to the <with|font-family|tt|graph> command. An
  undirected unweighted graph will be returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("coxeter")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 28 vertices and 42 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/cox.eps|35%|||>>

  <section|Cycle and path graphs>

  <subsection|Cycle graphs>

  The command<index|<verbatim|cycle_graph>> <with|font-family|tt|cycle_graph>
  is used for constructing <hlink|<rigid|cycle
  graphs>|https://en.wikipedia.org/wiki/Cycle_graph>
  <cite-detail|godsil|pp.<nbsp>4>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|cycle_graph(n)>>>|<row|<cell|>|<cell|<verbatim|cycle_graph(V)>>>>>>

  <kbd|cycle_graph> takes a positive integer <math|n> or a list of distinct
  vertices <math|V> as its only argument and returns the graph consisting of
  a single cycle on the specified vertices in the given order. If <math|n> is
  specified it is assumed to be the desired number of vertices, in which case
  they will be created and labeled with the first <math|n> integers (starting
  from 0 in <samp|Xcas> mode and from 1 in <samp|Maple> mode). The resulting
  graph will be given the name <kbd|Cn>, for example <kbd|C4> for <math|n=4>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C5:=cycle_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|C5: an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(C5)
    </input>
  </session>

  <center|<image|images/c5.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cycle_graph(["a","b","c","d","e"])
    <|unfolded-io>
      <\equation*>
        <text|C5: an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Path graphs>

  The command<index|<verbatim|path_graph>> <with|font-family|tt|path_graph>
  is used for constructing <hlink|<rigid|path
  graphs>|https://en.wikipedia.org/wiki/Path_graph>
  <cite-detail|godsil|pp.<nbsp>4>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|path_graph(n)>>>|<row|<cell|>|<cell|<verbatim|path_graph(V)>>>>>>

  <kbd|path_graph> takes a positive integer <math|n> or a list of distinct
  vertices <math|V> as its only argument and returns a graph consisting of a
  single path on the specified vertices in the given order. If <math|n> is
  specified it is assumed to be the desired number of vertices, in which case
  they will be created and labeled with the first <math|n> integers (starting
  from 0 in <samp|Xcas> mode resp.<nbsp>from 1 in <samp|Maple> mode).

  Note that a path cannot intersect itself. Paths that are allowed to cross
  themselves are called <strong|trails> (see the command
  <verbatim|<hlink|trail|#trail>>).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      path_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      path_graph(["a","b","c","d","e"])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Trails of edges><label|trail>

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|trail(v1,v2,..,vk)>>>|<row|<cell|>|<cell|<verbatim|trail2edges(T)>>>>>>

  If the dummy command<index|<verbatim|trail>> <with|font-family|tt|trail> is
  called with a sequence of vertices <math|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|k>>
  as arguments, it returns the symbolic expression representing the
  <hlink|trail|https://en.wikipedia.org/wiki/Path_(graph_theory)#Definitions>
  which visits the specified vertices in the given order. The resulting
  symbolic object is recognizable by some commands, for example
  <verbatim|<hlink|graph|#graph>> and <verbatim|<hlink|digraph|#digraph>>.
  Note that a trail may cross itself (some vertices may be repeated in the
  sequence).

  Any trail <math|T> is easily converted to the corresponding list of edges
  by calling the <verbatim|trail2edges><index|<verbatim|trail2edges>>
  command, which takes the trail as its only argument.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=trail(1,2,3,4,2):; graph(T)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<text|an undirected unweighted graph with 4 vertices
        and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      trail2edges(T)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|2>|<cell|3>>|<row|<cell|3>|<cell|4>>|<row|<cell|4>|<cell|2>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <section|Complete graphs>

  <subsection|Complete (multipartite) graphs>

  The command<index|<verbatim|complete_graph>>
  <with|font-family|tt|complete_graph> is used for construction of
  <hlink|complete|https://en.wikipedia.org/wiki/Complete_graph>
  (<hlink|multipartite|https://en.wikipedia.org/wiki/Multipartite_graph>)
  graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|1|3|3|cell-row-span|2>|<cwith|1|1|3|3|cell-col-span|1>|<cwith|1|-1|3|3|cell-lsep|1em>|<cwith|1|-1|3|3|cell-valign|c>|<table|<row|<cell|Syntax:>|<cell|<verbatim|complete_graph(n)>>|<cell|<very-small|complete
  graphs>>>|<row|<cell|>|<cell|<verbatim|complete_graph(V)>>|<cell|>>|<row|<cell|>|<cell|<verbatim|complete_graph(n1,n2,..,nk)>>|<cell|<very-small|complete
  multipartite graphs>>>>>>

  <with|font-family|tt|complete_graph> can be called with a single argument,
  a positive integer <math|n> or a list of distinct vertices <math|V>, in
  which case it returns the complete graph <cite-detail|godsil|pp.<nbsp>2> on
  the specified vertices. If integer <math|n> is specified, it is assumed
  that it is the desired number of vertices and they will be created and
  labeled with the first <math|n> integers (starting from 0 in <samp|Xcas>
  mode and from 1 in <samp|Maple> mode).

  If <with|font-family|tt|complete_graph> is given a sequence of positive
  integers <math|n<rsub|1>,n<rsub|2>,\<ldots\>,n<rsub|k>> as its argument, it
  returns a complete multipartite graph with partitions of size
  <math|n<rsub|1>,n<rsub|2>,\<ldots\>,n<rsub|k>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(K5)
    </input>
  </session>

  <center|<image|images/complete.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K3:=complete_graph([a,b,c])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(K3)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|a,b|]>,<around|[|a,c|]>,<around|[|b,c|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K33:=complete_graph(3,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(K33)
    </input>
  </session>

  <center|<image|images/k33.eps|35%|||>>

  <subsection|Complete trees>

  The commands <verbatim|complete_binary_tree><index|<verbatim|complete_binary_tree>>
  and <verbatim|complete_kary_tree><index|<verbatim|complete_kary_tree>> are
  used for construction of complete <hlink|<rigid|binary
  trees>|https://en.wikipedia.org/wiki/Binary_tree> and complete
  <hlink|<rigid|<math|k>-ary trees>|https://en.wikipedia.org/wiki/K-ary_tree>,
  respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|complete_binary_tree(n)>>>|<row|<cell|>|<cell|<verbatim|complete_kary_tree(k,n)>>>>>>

  <with|font-family|tt|complete_binary_tree> takes a positive integer
  <math|n> as its only argument and returns a complete binary tree of depth
  <math|n>.

  <with|font-family|tt|complete_kary_tree> takes positive integers <math|k>
  and <math|n> as its arguments and returns the complete <math|k>-ary tree of
  depth <math|n>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T1:=complete_binary_tree(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T1)
    </input>
  </session>

  <center|<image|images/ctree1.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T2:=complete_kary_tree(3,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 13 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T2)
    </input>
  </session>

  <center|<image|images/ctree2.eps|35%|||>>

  <section|Sequence graphs>

  <subsection|Creating graphs from degree sequences>

  The command<index|<verbatim|sequence_graph>>
  <with|font-family|tt|sequence_graph> is used for constructing graphs from
  <hlink|<rigid|degree sequences>|https://en.wikipedia.org/wiki/Degree_(graph_theory)#Degree_sequence>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|sequence_graph(L)>>>>>>

  <kbd|sequence_graph> takes a list <math|L> of positive integers as its only
  argument and, if <math|L> represents a graphic sequence, the corresponding
  graph <math|G> with <math|<around*|\||L|\|>> <math|vertices> is returned.
  If the argument is not a graphic sequence, an error is returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      sequence_graph([3,2,4,2,3,4,5,7])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 15 edges>
      </equation*>
    </unfolded-io>
  </session>

  Sequence graphs are constructed in <math|O<around|(|<around|\||L|\|><rsup|2>*log
  <around|\||L|\|>|)>> time by applying the algorithm of <name|Havel> and
  <name|Hakimi><nbsp><cite|hakimi>.

  <subsection|Validating graphic sequences>

  The command<index|<verbatim|is_graphic_sequence>>
  <with|font-family|tt|is_graphic_sequence> is used to check whether a list
  of integers represents the degree sequence of some graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_graphic_sequence(L)>>>>>>

  <with|font-family|tt|is_graphic_sequence> takes a list <math|L> of positive
  integers as its only argument and returns <with|font-family|tt|true> if
  there exists a graph <math|G<around|(|V,E|)>> with degree sequence
  <math|<around|{|deg v:v\<in\>V|}>> equal to <math|L> and
  <with|font-family|tt|false> otherwise. The algorithm, which has the
  complexity <math|O<around|(|<around|\||L|\|><rsup|2>|)>>, is based on the
  <hlink|theorem|https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93Gallai_theorem>
  of <name|Erd®s> and <name|Gallai>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_graphic_sequence([3,2,4,2,3,4,5,7])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <section|Intersection graphs>

  <subsection|Interval graphs>

  The command<index|<verbatim|interval_graph>>
  <with|font-family|tt|interval_graph> is used for construction of
  <hlink|<rigid|interval graphs>|https://en.wikipedia.org/wiki/Interval_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|interval_graph(L)>>>>>>

  <kbd|interval_graph> takes a sequence or list <math|L> of real-line
  intervals as its argument and returns an undirected unweighted graph with
  these intervals as vertices (the string representations of the intervals
  are used as labels), each two of them being connected with an edge if and
  only if the corresponding intervals intersect.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=interval_graph(0..8,1..pi,exp(1)..20,7..18,11..14,17..24,23..25)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/intersect.png|35%|||>>

  <subsection|Kneser graphs>

  The commands<index|<verbatim|kneser_graph>>
  <with|font-family|tt|kneser_graph> and<index|<verbatim|odd_graph>>
  <kbd|odd_graph> are used for construction of <hlink|<rigid|Kneser
  graphs>|https://en.wikipedia.org/wiki/Kneser_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|kneser_graph(n,k)>>>|<row|<cell|>|<cell|<verbatim|odd_graph(d)>>>>>>

  <kbd|kneser_graph> takes two positive integers <math|n\<leq\>20> and
  <math|k> as its arguments and returns the Kneser graph
  <math|K<around*|(|n,k|)>>. The latter is obtained by setting all
  <math|k>-subsets of a set of <math|n> elements as vertices and connecting
  each two of them if and only if the corresponding sets are disjoint.
  Therefore, each Kneser graph is the complement of the corresponding
  intersection graph on the same collection of subsets.

  Kneser graphs can get exceedingly complex even for relatively small values
  of <math|n> and <math|k>. Note that the number of vertices in
  <math|K<around*|(|n,k|)>> is equal to <math|<binom|n|k>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      kneser_graph(5,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=kneser_graph(8,1)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 28 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_clique(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/kneser.eps|35%|||>>

  The command <with|font-family|tt|odd_graph> is used for creating
  <strong|odd graphs>, i.e.<nbsp>Kneser graphs with parameters <math|n=2*d+1>
  and <math|k=d> for <math|d\<geqslant\>1>.

  <with|font-family|tt|odd_graph> takes a positive integer <math|d\<leq\>8>
  as its only argument and returns <math|d>-th odd graph
  <math|K<around*|(|2*d+1,d|)>>. Note that the odd graphs with
  <math|d\<gtr\>8> will not be constructed as they are too big to handle.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      odd_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Special graphs>

  <subsection|Hypercube graphs>

  The command<index|<verbatim|hypercube_graph>> <verbatim|hypercube_graph> is
  used for construction of <hlink|<rigid|hypercube
  graphs>|https://en.wikipedia.org/wiki/Hypercube_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|hypercube_graph(n)>>>>>>

  <verbatim|hypercube_graph> takes a positive integer <math|n> as its only
  argument and returns the hypercube graph of dimension <math|n> on
  <math|2<rsup|n>> vertices. The vertex labels are strings of binary digits
  of length <math|n>. Two vertices are joined by an edge if and only if their
  labels differ in exactly one character. The hypercube graph for <math|n=2>
  is a square and for <math|n=3> it is a cube.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=hypercube_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,planar)
    </input>
  </session>

  <center|<image|images/cubeplan.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=hypercube_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 32 vertices and 80 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,plot3d,labels=false)
    </input>
  </session>

  <center|<image|images/hc.eps|30%|||>>

  <subsection|Star graphs>

  The command<index|<verbatim|star_graph>> <with|font-family|tt|star_graph>
  is used for construction of <hlink|<rigid|star
  graphs>|https://en.wikipedia.org/wiki/Star_(graph_theory)>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|star_graph(n)>>>>>>

  <with|font-family|tt|star_graph> takes a positive integer <math|n> as its
  only argument and returns the star graph with <math|n+1> vertices, which is
  equal to the complete bipartite graph <with|font-family|tt|complete_graph(1,n)>
  i.e.<nbsp>a <math|n>-ary tree with one level.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=star_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/star.eps|35%|||>>

  <subsection|Wheel graphs>

  The command<index|<verbatim|wheel_graph>> <with|font-family|tt|wheel_graph>
  is used for construction of <hlink|<rigid|wheel
  graphs>|https://en.wikipedia.org/wiki/Wheel_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|wheel_graph(n)>>>>>>

  <with|font-family|tt|wheel_graph> takes a positive integer <math|n> as its
  only argument and returns the wheel graph with <math|n+1> vertices.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=wheel_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/wheel.eps|35%|||>>

  <subsection|Web graphs>

  The command<index|<verbatim|web_graph>> <with|font-family|tt|web_graph> is
  used for construction of web graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|web_graph(a,b)>>>>>>

  <with|font-family|tt|web_graph> takes two positive integers <math|a> and
  <math|b> as its arguments and returns the web graph with parameters
  <math|a> and <math|b>, namely the Cartesian product of
  <with|font-family|tt|cycle_graph(a)> and
  <with|font-family|tt|path_graph(b)>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=web_graph(7,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 21 vertices and 35 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,labels=false)
    </input>
  </session>

  <center|<image|images/web.eps|35%|||>>

  <subsection|Prism graphs>

  The command<index|<verbatim|prism_graph>> <with|font-family|tt|prism_graph>
  is used for construction of <hlink|<rigid|prism
  graphs>|https://en.wikipedia.org/wiki/Prism_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|prism_graph(n)>>>>>>

  <with|font-family|tt|prism_graph> takes a positive integer <math|n> as its
  only argument and returns the prism graph with parameter <math|n>, namely
  <with|font-family|tt|petersen_graph(n,1)>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=prism_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/prism.eps|35%|||>>

  <subsection|Antiprism graphs>

  The command<index|<verbatim|antiprism_graph>>
  <with|font-family|tt|antiprism_graph> is used for construction of
  <hlink|<rigid|antiprism graphs>|https://en.wikipedia.org/wiki/Antiprism_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|antiprism_graph(n)>>>>>>

  <with|font-family|tt|antiprism_graph> takes a positive integer <math|n> as
  its only argument and returns the antiprism graph with parameter <math|n>,
  which is constructed from two concentric cycles of <math|n> vertices by
  joining each vertex of the inner to two adjacent nodes of the outer cycle.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=antiprism_graph(7)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 14 vertices and 28 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/antiprism.eps|35%|||>>

  <subsection|Grid graphs>

  The command<index|<verbatim|grid_graph>> <with|font-family|tt|grid_graph>
  resp.<nbsp><verbatim|torus_grid_graph><index|<verbatim|torus_grid_graph>>
  is used for construction of rectangular/triangular resp.<nbsp>torus
  <hlink|<rigid|grid graphs>|https://en.wikipedia.org/wiki/Lattice_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|-1|3|3|cell-lsep|1em>|<cwith|1|-1|3|3|cell-valign|c>|<table|<row|<cell|Syntax:>|<cell|<verbatim|grid_graph(m,n)>>|<cell|<very-small|rectangular
  grids>>>|<row|<cell|>|<cell|<verbatim|grid_graph(m,n,triangle)>>|<cell|<very-small|triangular
  grids>>>|<row|<cell|>|<cell|<verbatim|torus_grid_graph(m,n)>>|<cell|<very-small|toroidal
  grids>>>>>>

  <with|font-family|tt|grid_graph> takes two positive integers <math|m> and
  <math|n> as its arguments and returns the <math|m> by <math|n> grid on
  <math|m*n> vertices, namely the Cartesian product of
  <with|font-family|tt|path_graph(m)> and
  <with|font-family|tt|path_graph(n)>. If the option <verbatim|triangle> is
  passed as the third argument, the returned graph is a triangular grid on
  <math|m*n> vertices defined as the underlying graph of the
  <hlink|<rigid|strong product>|https://en.wikipedia.org/wiki/Strong_product_of_graphs>
  of two directed path graphs with <math|m> and <math|n> vertices,
  respectively<nbsp><cite-detail|afzal|Definition<nbsp>2, pp.<nbsp>189>.
  Strong product is defined as the <hlink|union|#graph-union> of Cartesian
  and tensor <hlink|products|#graph-products>.

  <with|font-family|tt|torus_grid_graph> takes two positive integers <math|m>
  and <math|n> as its arguments and returns the <math|m> by <math|n> torus
  grid on <math|m*n> vertices, namely the Cartesian product of
  <with|font-family|tt|cycle_graph(m)> and
  <with|font-family|tt|cycle_graph(n)>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=grid_graph(15,20)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 300 vertices and 565 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/grid2.eps|35%|||>>

  For example, connecting vertices in the opposite corners of the above grid
  yields a grid-like graph with no corners.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=add_edge(G,[["14:0","0:19"],["0:0","14:19"]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 300 vertices and 567 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,plot3d)
    </input>
  </session>

  <center|<image|images/net.eps|35%|||>>

  In the next example, the Möbius strip is constructed by connecting the
  vertices in the opposite sides of a narrow grid graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=grid_graph(20,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 97 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=add_edge(G,[["0:0","19:2"],["0:1","19:1"],["0:2","19:0"]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 100 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,plot3d,labels=false)
    </input>
  </session>

  <center|<image|images/strip.eps|35%|||>>

  A triangular grid is created by passing the option <verbatim|triangle>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=grid_graph(10,15,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 150 vertices and 401 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/trigrid.eps|35%|||>>

  The next example demonstrates creating a torus grid graph with eight
  triangular levels.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=torus_grid_graph(8,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 24 vertices and 48 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/torusgrid.eps|35%|||>>

  <subsection|Sierpi«ski graphs>

  The command<index|<verbatim|sierpinski_graph>>
  <with|font-family|tt|sierpinski_graph> is used for construction of
  Sierpi«ski-type graphs <math|S<rsub|k><rsup|n>> and <math|S
  T<rsub|k><rsup|n>><nbsp><cite|hinz>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|sierpinski_graph(n,k)>>>|<row|<cell|>|<cell|<verbatim|sierpinski_graph(n,k,triangle)>>>>>>

  <with|font-family|tt|sierpinski_graph> takes two positive integers <math|n>
  and <math|k> as its arguments and optionally the option
  <with|font-family|tt|triangle> as the third argument. It returns the
  Sierpi«ski (triangle) graph with parameters <math|n> and <math|k>.

  The Sierpi«ski triangle graph <math|S T<rsub|k><rsup|n>> is obtained by
  contracting all non-clique edges in <math|S<rsub|k><rsup|n>>. To detect
  such edges the variant of the algorithm by <name|Bron> and <name|Kerbosch>,
  developed by <name|Tomita> et al.<nbsp>in <cite|tomita>, is used, which can
  be time consuming for <math|n\<gtr\>6>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=sierpinski_graph(4,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 81 vertices and 120 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(S,spring)
    </input>
  </session>

  <center|<image|images/s43.eps|35%|||>>

  In particular, <math|S T<rsub|3><rsup|n>> is the well-known
  <hlink|<rigid|Sierpi«ski sieve>|https://en.wikipedia.org/wiki/Sierpinski_triangle>
  graph of order <math|n>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      sierpinski_graph(4,3,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 42 vertices and 81 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      sierpinski_graph(5,3,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 123 vertices and 243 edges>
      </equation*>
    </unfolded-io>
  </session>

  A drawing of the graph produced by the last command line is shown in
  Figure<nbsp><reference|st53>.

  <subsection|Generalized Petersen graphs>

  The command<index|<verbatim|petersen_graph>>
  <with|font-family|tt|petersen_graph> is used for construction of
  <hlink|<rigid|generalized Petersen graphs>|https://en.wikipedia.org/wiki/Generalized_Petersen_graph>
  <math|P<around*|(|n,k|)>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|petersen_graph(n)>>>|<row|<cell|>|<cell|<verbatim|petersen_graph(n,k)>>>>>>

  <with|font-family|tt|petersen_graph> takes two positive integers <math|n>
  and <math|k> as its arguments. The second argument may be omitted, in which
  case <math|k=2> is assumed. The graph <math|P<around|(|n,k|)>>, which is
  returned, is a connected cubic graph consisting of\Vin Schläfli
  notation\Van inner star polygon <math|<around|{|n,k|}>> and an outer
  regular polygon <math|<around|{|n|}>> such that the <math|n> pairs of
  corresponding vertices in inner and outer polygons are connected with
  edges. For <math|k=1> the prism graph of order <math|n> is obtained.

  The well-known Petersen graph is equal to the generalized Petersen graph
  <math|P<around*|(|5,2|)>>. It can also be constructed by calling
  <verbatim|graph("petersen")>.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(graph("petersen"))
    </input>
  </session>

  <center|<image|images/petersen.eps|35%|||>>

  To obtain the dodecahedral graph <math|P<around|(|10,2|)>>, input:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      petersen_graph(10)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>
  </session>

  To obtain Möbius\UKantor graph <math|P<around*|(|8,3|)>>, input:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=petersen_graph(8,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 24 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/moka.eps|35%|||>>

  Note that Desargues, Dürer and Nauru graphs are isomorphic to the
  generalized Petersen graphs <math|P<around|(|10,3|)>>,
  <math|P<around|(|6,2|)>> and <math|P<around|(|12,5|)>>, respectively.

  <subsection|<abbr|LCF> graphs>

  The command<index|<verbatim|lcf_graph>> <verbatim|lcf_graph> is used for
  construction of cubic Hamiltonian graphs from <hlink|<rigid|<abbr|LCF>
  notation>|https://en.wikipedia.org/wiki/LCF_notation>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|lcf_graph(L)>>>|<row|<cell|>|<cell|<verbatim|lcf_graph(L,n)>>>>>>

  <verbatim|lcf_graph> takes one or two arguments, a list <math|L> of nonzero
  integers, called <em|jumps>, and optionally a positive integer <math|n>,
  called the <em|exponent> (by default, <math|n=1>). The command returns the
  graph on <math|n*<around*|\||L|\|>> vertices obtained by iterating the
  sequence of jumps <math|n> times.

  For example, the following command line creates <hlink|Frucht
  graph|https://en.wikipedia.org/wiki/Frucht_graph><label|frucht>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      F:=lcf_graph([-5,-2,-4,2,5,-2,2,5,-2,-5,4,2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 12 vertices and 18 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(F,planar)
    </input>
  </session>

  <center|<image|images/frucht.eps|35%|||>>

  In the next example, the truncated octahedral graph is constructed from
  <abbr|LCF> notation.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=lcf_graph([3,-7,7,-3],6)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 24 vertices and 36 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar,labels=false)
    </input>
  </session>

  <center|<image|images/oct_trunc.eps|35%|||>>

  <section|Isomorphic copies of graphs>

  <subsection|Creating isomorphic copies from
  permutations><label|isomorphic-copy>

  To create an isomorphic copy of a graph use the
  <verbatim|isomorphic_copy><index|<kbd|isomorphic_copy>> command.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|isomorphic_copy(G,sigma)>>>|<row|<cell|>|<cell|<verbatim|isomorphic_copy(G)>>>>>>

  <with|font-family|tt|isomorphic_copy> takes one or two arguments, a graph
  <math|G<around|(|V,E|)>> and optionally a permutation <math|\<sigma\>> of
  order <math|<around|\||V|\|>>. It returns a new graph where the adjacency
  lists are reordered according to <math|\<sigma\>> or a random permutation
  if the second argument is omitted. The vertex labels are the same as in
  <math|G>. This command discards all vertex and edge attributes present in
  <math|G>.

  The complexity of the algorithm is <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=path_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(G), neighbors(G)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,3,4,5|]>,<around|{|<around|[|2|]>,<around|[|1,3|]>,<around|[|2,4|]>,<around|[|3,5|]>,<around|[|4|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=isomorphic_copy(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(H), neighbors(H)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,3,4,5|]>,<around|{|<around|[|2,3|]>,<around|[|1,5|]>,<around|[|1,4|]>,<around|[|3|]>,<around|[|2|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=isomorphic_copy(G,[2,4,0,1,3])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(H), neighbors(H)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,3,4,5|]>,<around|{|<around|[|4,5|]>,<around|[|5|]>,<around|[|4|]>,<around|[|1,3|]>,<around|[|1,2|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=prism_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(P)
    </input>
  </session>

  <center|<image|images/prism2.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=isomorphic_copy(P,[3,0,1,5,4,2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,spring)
    </input>
  </session>

  <center|<image|images/prism3.eps|35%|||>>

  <subsection|Permuting vertices>

  To create an isomorphic copy of a graph by providing the reordered list of
  vertex labels, use the command <verbatim|permute_vertices><index|<kbd|permute_vertices>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|permute_vertices(G,L)>>>|<row|<cell|>|<cell|<verbatim|permute_vertices(G)>>>>>>

  <with|font-family|tt|permute_vertices> takes one or two arguments, a graph
  <math|G<around|(|V,E|)>> and optionally a list <math|L> of length
  <math|<around|\||V|\|>> containing all vertices from <math|V>, and returns
  a copy of <math|G> with vertices rearranged in order they appear in
  <math|L> or at random if <math|L> is not given. All vertex and edge
  attributes are copied, which includes vertex position information (if
  present). That means the resulting graph will look the same as <math|G>
  when drawn.

  The complexity of the algorithm is <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=path_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(G), neighbors(G)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,3,4,5|]>,<around|{|<around|[|2|]>,<around|[|1,3|]>,<around|[|2,4|]>,<around|[|3,5|]>,<around|[|4|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=permute_vertices(G,[3,5,1,2,4])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(H), neighbors(H)
    <|unfolded-io>
      <\equation*>
        <around|[|3,5,1,2,4|]>,<around|{|<around|[|2,4|]>,<around|[|4|]>,<around|[|2|]>,<around|[|1,3|]>,<around|[|3,5|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Relabeling vertices>

  To relabel the vertices of a graph without changing their order, use the
  command <verbatim|relabel_vertices><index|<kbd|relabel_vertices>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|relabel_vertices(G,L)>>>>>>

  <with|font-family|tt|relabel_vertices> takes two arguments, a graph
  <math|G<around|(|V,E|)>> and a list <math|L> of vertex labels of length
  <math|<around*|\||V|\|>>. It returns a copy of <math|G> with <math|L> as
  the list of vertex labels.

  The complexity of the algorithm is <math|O<around*|(|<around*|\||V|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=path_graph([1,2,3,4])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|2,3|]>,<around|[|3,4|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=relabel_vertices(G,[a,b,c,d])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(H)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|a,b|]>,<around|[|b,c|]>,<around|[|c,d|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <section|Subgraphs>

  <subsection|Extracting subgraphs><label|subgraph>

  To extract the subgraph of a graph formed by a subset of the set of its
  edges, use the command <with|font-family|tt|subgraph><index|<with|font-family|tt|subgraph>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|subgraph(G,L)>>>>>>

  <verbatim|subgraph> takes two arguments, a graph <math|G<around*|(|V,E|)>>
  and a list of edges <math|L>. It returns the subgraph
  <math|G<rprime|'><around*|(|V<rprime|'>,L|)>> of <math|G>, where
  <math|V<rprime|'>\<subset\>V> is a subset of vertices of <math|G> incident
  to at least one edge from <math|L>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=subgraph(K5,[[1,2],[2,3],[3,4],[4,1]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(K5,S))
    </input>
  </session>

  <center|<image|images/sg0.eps|35%|||>>

  <subsection|Induced subgraphs><label|induced-subgraph>

  To obtain the subgraph of a graph <hlink|induced|https://en.wikipedia.org/wiki/Induced_subgraph>
  by a subset of its vertices, use the command
  <with|font-family|tt|induced_subgraph><index|<with|font-family|tt|induced_subgraph>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|induced_subgraph(G,L)>>>>>>

  <with|font-family|tt|induced_subgraph> takes two arguments, a graph
  <math|G<around*|(|V,E|)>> and a list of vertices <math|L>. It returns the
  subgraph <math|G<rprime|'><around*|(|L,E<rprime|'>|)>> of <math|G>, where
  <math|E<rprime|'>\<subset\>E> contains all edges which have both endpoints
  in <math|L> <cite-detail|godsil|pp.<nbsp>3>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=induced_subgraph(G,[5,6,7,8,9])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(G,S))
    </input>
  </session>

  <center|<image|images/sg.eps|35%|||>>

  <subsection|Underlying graphs><label|underlying-graph>

  For every graph <math|G<around*|(|V,E|)>> there is an undirected and
  unweighted graph <math|U<around*|(|V,E<rprime|'>|)>>, called the
  <strong|underlying graph> of <math|G>, where <math|E<rprime|'>> is obtained
  from <math|E> by dropping edge directions. To construct <math|U>, use the
  command <with|font-family|tt|underlying_graph><index|<with|font-family|tt|underlying_graph>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|underlying_graph(G)>>>>>>

  <verbatim|underlying_graph> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns an undirected unweighted copy of <math|G> in
  which all vertex and edge attributes, together with edge directions, are
  discarded.

  The complexity of the algorithm is <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(%{[[1,2],6],[[2,3],4],[[3,1],5],[[3,2],7]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 3 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      U:=underlying_graph(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(U)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|1,3|]>,<around|[|2,3|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Fundamental cycles>

  The command <verbatim|fundamental_cycle><index|<verbatim|fundamental_cycle>>
  is used for extracting cycles from <hlink|<rigid|unicyclic
  graphs>|https://en.wikipedia.org/wiki/Pseudoforest#Definitions_and_structure>
  (also called 1-trees). To find a <hlink|<rigid|fundamental cycle
  basis>|https://en.wikipedia.org/wiki/Cycle_basis#Fundamental_cycles> of an
  undirected graph, use the command <verbatim|cycle_basis><index|<verbatim|cycle_basis>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|fundamental_cycle(G)>>>|<row|<cell|>|<cell|<verbatim|cycle_basis(G)>>>>>>

  <verbatim|fundamental_cycle> takes one argument, an undirected connected
  graph <math|G<around*|(|V,E|)>> containing exactly one cycle (i.e.<nbsp>a
  unicyclic graph), and returns that cycle as a graph. If <math|G> is not
  unicyclic, an error is returned.

  <verbatim|cycle_basis> takes an undirected graph <math|G<around*|(|V,E|)>>
  as its only argument and returns a basis <math|B> of the cycle space of
  <math|G> as a list of fundamental cycles in <math|G>, with each cycle
  represented as a list of vertices. Furthermore,
  <math|<around*|\||B|\|>=<around*|\||E|\|>-<around*|\||V|\|>+\<kappa\><around*|(|G|)>>,
  where <math|\<kappa\><around*|(|G|)>> is the number of connected components
  of <math|G>. Every cycle <math|C> in <math|G> such that <math|C\<nin\>B>
  can be obtained from cycles in <math|B> using only <hlink|<rigid|symmetric
  differences>|https://en.wikipedia.org/wiki/Symmetric_difference>.

  The strategy is to construct a spanning tree <math|T> of <math|G> using
  depth-first search and look for edges in <math|E> which do not belong to
  the tree. For each non-tree edge <math|e> there is a unique fundamental
  cycle <math|C<rsub|e>> consisting of <math|e> together with the path in
  <math|T> connecting the endpoints of <math|e>. The vertices of
  <math|C<rsub|e>> are easily obtained from the search data. The complexity
  of this algorithm is <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5,2,6))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=fundamental_cycle(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(C)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|5>>|<row|<cell|2>|<cell|3>>|<row|<cell|4>|<cell|5>>|<row|<cell|3>|<cell|4>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  Given a tree graph <math|G> and adding an edge from the complement
  <math|G<rsup|c>> to <math|G> one obtains a 1-tree graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tree(25)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 25 vertices and 24 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      ed:=choice(edges(graph_complement(G)))
    <|unfolded-io>
      <\equation*>
        <around|[|10,20|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=add_edge(G,ed)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 25 vertices and 25 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=fundamental_cycle(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(C)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|10>|<cell|20>>|<row|<cell|0>|<cell|10>>|<row|<cell|1>|<cell|20>>|<row|<cell|1>|<cell|2>>|<row|<cell|2>|<cell|24>>|<row|<cell|13>|<cell|24>>|<row|<cell|7>|<cell|13>>|<row|<cell|0>|<cell|7>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(G,C),spring)
    </input>
  </session>

  <center|<image|images/ftree.eps|35%|||>>

  In the next example, a cycle basis of octahedral graph is computed.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/octa.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cycle_basis(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|6,3,1|]>,<around|[|5,4,6,3,1|]>,<around|[|4,6,3,1|]>,<around|[|5,4,6,3|]>,<around|[|2,5,4,6,3|]>,<around|[|2,5,4,6|]>,<around|[|2,5,4|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  Given a tree graph <math|T>, one can create a graph with cycle basis
  cardinality equal to <math|k> by simply adding <math|k> randomly selected
  edges from the complement <math|T<rsup|c>> to <math|T>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tree1:=random_tree(15)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=add_edge(tree1,rand(3,edges(graph_complement(tree1))))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 17 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tree2:=random_tree(12)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 12 vertices and 11 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=add_edge(tree2,rand(2,edges(graph_complement(tree2))))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 12 vertices and 13 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=disjoint_union(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 27 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <\center>
    <image|images/ftrees.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      nops(cycle_basis(G))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_edges(G)-number_of_vertices(G)+nops(connected_components(G))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>
  </session>

  <section|Operations on graphs>

  <subsection|Graph complement>

  The command <with|font-family|tt|graph_complement><index|<with|font-family|tt|graph_complement>>
  is used for construction of <hlink|<rigid|complement
  graphs>|https://en.wikipedia.org/wiki/Complement_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_complement(G)>>>>>>

  <with|font-family|tt|graph_complement> takes a graph
  <math|G<around|(|V,E|)>> as its only argument and returns the complement
  graph <math|G<rsup|c><around|(|V,E<rsup|c>|)>> of <math|G>, where
  <math|E<rsup|c>> is the largest set containing only edges/arcs not present
  in <math|G>. The complexity of the algorithm is
  <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C5:=cycle_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|C5: an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(C5)
    </input>
  </session>

  <center|<image|images/c5.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_complement(C5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/star2.eps|35%|||>>

  <subsection|Seidel switching>

  The command <with|font-family|tt|seidel_switch><index|<with|font-family|tt|seidel_switch>>
  is used for Seidel <hlink|switching|https://en.wikipedia.org/wiki/Two-graph#Switching_and_graphs>
  in graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|seidel_switch(G,L)>>>>>>

  <with|font-family|tt|seidel_switch> takes two arguments, an undirected and
  unweighted graph <math|G<around|(|V,E|)>> and a list of vertices
  <math|L\<subset\>V>. The result is a copy of <math|G> in which, for each
  vertex <math|v\<in\>L>, its neighbors become its non-neighbors and vice
  versa.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=seidel_switch(cycle_graph(5),[1,2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(S)
    </input>
  </session>

  <center|<image|images/seidel.eps|35%|||>>

  <subsection|Transposing graphs>

  The command <with|font-family|tt|reverse_graph><index|<with|font-family|tt|reverse_graph>>
  is used for reversing arc directions in digraphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|reverse_graph(G)>>>>>>

  <with|font-family|tt|reverse_graph> takes a graph <math|G<around|(|V,E|)>>
  as its only argument and returns the reverse graph
  <math|G<rsup|T><around|(|V,E<rprime|'>|)>> of <math|G> where
  <math|E<rprime|'>=<around|{|<around|(|j,i|)>:<around|(|i,j|)>\<in\>E|}>>,
  i.e.<nbsp>returns the copy of <math|G> with the directions of all edges
  reversed.

  Note that <with|font-family|tt|reverse_graph> is defined for both directed
  and undirected graphs, but gives meaningful results only for directed
  graphs.

  <math|G<rsup|T>> is also called the <strong|transpose graph> of <math|G>
  because adjacency matrices of <math|G> and <math|G<rsup|T>> are transposes
  of each other (hence the notation).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(6, %{[1,2],[2,3],[2,4],[4,5]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 6 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      GT:=reverse_graph(G)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 6 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(GT)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|2,1|]>,<around|[|3,2|]>,<around|[|4,2|]>,<around|[|5,4|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Union of graphs><label|graph-union>

  The command <with|font-family|tt|graph_union><index|<with|font-family|tt|graph_union>>
  is used for constructing unions of graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_union(G1,G2,..,Gn)>>>>>>

  <with|font-family|tt|graph_union> takes a sequence of graphs
  <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the graph
  <math|G<around|(|V,E|)>> where <math|V=V<rsub|1>\<cup\>V<rsub|2>\<cup\>\<cdots\>\<cup\>V<rsub|k>>
  and <math|E=E<rsub|1>\<cup\>E<rsub|2>\<cup\>\<cdots\>\<cup\>E<rsub|k>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph([1,2,3],%{[1,2],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=graph([1,2,3],%{[3,1],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_union(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|1,3|]>,<around|[|2,3|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Disjoint union of graphs>

  To construct <hlink|<rigid|disjoint union>|https://en.wikipedia.org/wiki/Disjoint_union_of_graphs>
  of graphs use the command <with|font-family|tt|disjoint_union><index|<with|font-family|tt|disjoint_union>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|disjoint_union(G1,G2,..,Gn)>>>>>>

  <with|font-family|tt|disjoint_union> takes a sequence of graphs
  <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its only argument and returns the graph
  obtained by labeling all vertices with strings <kbd|k:v> where
  <math|v\<in\>V<rsub|k>> and all edges with strings <kbd|k:e> where
  <math|e\<in\>E<rsub|k>> and calling <verbatim|<hlink|graph_union|#graph-union>>
  subsequently. As all vertices and edges are labeled differently, it follows
  <math|<around|\||V|\|>=<big|sum><rsub|k=1><rsup|n><around|\||V<rsub|k>|\|>>
  and <math|<around|\||E|\|>=<big|sum><rsub|k=1><rsup|n><around|\||E<rsub|k>|\|>>.6

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=disjoint_union(cycle_graph([1,2,3]),path_graph([1,2,3]))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/disjoint.eps|35%|||>>

  <subsection|Joining two graphs>

  The command <with|font-family|tt|graph_join><index|<with|font-family|tt|graph_join>>
  is used for joining two graphs together.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_join(G,H)>>>>>>

  <with|font-family|tt|graph_join> takes two graphs <math|G> and <math|H> as
  its arguments and returns the graph <math|G+H> which is obtained by
  connecting all the vertices of <math|G> to all vertices of <math|H>. The
  vertex labels in the resulting graph are strings of the form <kbd|1:u> and
  <kbd|2:v> where <math|u> is a vertex in <math|G> and <math|v> is a vertex
  in <math|H>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=path_graph(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 2 vertices and 1 edge>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 0 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      GH:=graph_join(G,H)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(GH,spring)
    </input>
  </session>

  <center|<image|images/join.eps|35%|||>>

  <subsection|Power graphs>

  The command <with|font-family|tt|graph_power><index|<with|font-family|tt|graph_power>>
  is used for computing <hlink|<rigid|powers of
  graphs>|https://en.wikipedia.org/wiki/Graph_power>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_power(G,k)>>>>>>

  <with|font-family|tt|graph_power> takes two arguments, a graph
  <math|G<around|(|V,E|)>> and a positive integer <math|k>. It returns the
  <math|k>-th power <math|G<rsup|k>> of <math|G> with vertices <math|V> such
  that <math|v,w\<in\>V> are connected with an edge if and only if there
  exists a path of length at most <math|k> in <math|G>.

  The graph <math|G<rsup|k>> is constructed from its adjacency matrix
  <math|A<rsub|k>> which is obtained by adding powers of the adjacency matrix
  <math|A> of <math|G>:

  <\equation*>
    A<rsub|k>=<big|sum><rsub|i=1><rsup|k>A<rsup|k>.
  </equation*>

  The above sum is obtained by assigning <math|A<rsub|k>\<leftarrow\>A> and
  repeating the instruction <math|A<rsub|k>\<leftarrow\><around|(|A<rsub|k>+I|)>*A>
  for <math|k-1> times, so exactly <math|k> matrix multiplications are
  required.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle)
    </input>
  </session>

  <center|<image|images/pow1.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P2:=graph_power(G,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(P2,circle)
    </input>
  </session>

  <center|<image|images/pow2.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P3:=graph_power(G,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(P3,circle)
    </input>
  </session>

  <center|<image|images/pow3.eps|35%|||>>

  <subsection|Graph products><label|graph-products>

  There are two distinct operations for computing a product of two graphs:
  the <hlink|<rigid|Cartesian product>|https://en.wikipedia.org/wiki/Cartesian_product_of_graphs>
  and the <hlink|<rigid|tensor product>|https://en.wikipedia.org/wiki/Tensor_product_of_graphs>.
  These operations are available in <samp|Giac> as the commands
  <kbd|cartesian_product><index|<kbd|cartesian_product>> and
  <kbd|tensor_product><index|<kbd|tensor_product>>, respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|cartesian_product(G1,G2,..,Gn)>>>|<row|<cell|>|<cell|<verbatim|tensor_product(G1,G2,..,Gn)>>>>>>

  <with|font-family|tt|cartesian_product> takes a sequence of graphs
  <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the Cartesian product
  <math|G<rsub|1>\<times\>G<rsub|2>\<times\>\<cdots\>\<times\>G<rsub|n>> of
  the input graphs. The Cartesian product
  <math|G<around|(|V,E|)>=G<rsub|1>\<times\>G<rsub|2>> is the graph with list
  of vertices <math|V=V<rsub|1>\<times\>V<rsub|2>>, labeled with strings
  <kbd|v1:v2> where <math|v<rsub|1>\<in\>V<rsub|1>> and
  <math|v<rsub|2>\<in\>V<rsub|2>>, such that
  <math|<text|(<kbd|u1:v1>,<kbd|u2:v2>)>\<in\>E> if and only if
  <math|u<rsub|1>> is adjacent to <math|u<rsub|2>> and
  <math|v<rsub|1>=v<rsub|2>> <strong|or> <math|u<rsub|1>=u<rsub|2>> and
  <math|v<rsub|1>> is adjacent to <math|v<rsub|2>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph(trail(1,2,3,4,1,5))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G1,circle)
    </input>
  </session>

  <center|<image|images/graph4.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=star_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G2,circle=[1,2,3])
    </input>
  </session>

  <center|<image|images/graph3.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=cartesian_product(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 35 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/prod1.eps|35%|||>>

  <with|font-family|tt|tensor_product> takes a sequence of graphs
  <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the tensor product
  <math|G<rsub|1>\<times\>G<rsub|2>\<times\>\<cdots\>\<times\>G<rsub|n>> of
  the input graphs. The tensor product <math|G<around|(|V,E|)>=G<rsub|1>\<times\>G<rsub|2>>
  is the graph with list of vertices <math|V=V<rsub|1>\<times\>V<rsub|2>>,
  labeled with strings <kbd|v1:v2> where <math|v<rsub|1>\<in\>V<rsub|1>> and
  <math|v<rsub|2>\<in\>V<rsub|2>>, such that
  <math|<text|(<kbd|u1:v1>,<kbd|u2:v2>)>\<in\>E> if and only if
  <math|u<rsub|1>> is adjacent to <math|u<rsub|2>> <strong|and>
  <math|v<rsub|1>> is adjacent to <math|v<rsub|2>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=tensor_product(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T)
    </input>
  </session>

  <center|<image|images/prod2.eps|35%|||>>

  <subsection|Transitive closure graph>

  The command <verbatim|transitive_closure><index|<verbatim|transitive_closure>>
  is used for constructing <hlink|<rigid|transitive closure
  graphs>|https://en.wikipedia.org/wiki/Transitive_closure#In_graph_theory>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|transitive_closure(G)>>>|<row|<cell|>|<cell|<verbatim|transitive_closure(G,weighted[=true
  or false])>>>>>>

  <verbatim|transitive_closure> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally the option
  <verbatim|weighted=true> (or simply <verbatim|weighted>) or the option
  <verbatim|weighted=false> (which is the default). The command returns the
  transitive closure <math|T<around*|(|V,E<rprime|'>|)>> of the input graph
  <math|G> by connecting <math|u\<in\>V> to <math|v\<in\>V> in <math|T> if
  and only if there is a path from <math|u> to <math|v> in <math|G>. If
  <math|G> is directed, then <math|T> is also directed. When
  <verbatim|weighted=true> is specified, <math|T> is weighted such that the
  weight of edge <math|v\<space\>w\<in\>E<rprime|'>> is equal to the length
  (or cost, if <math|G> is weighted) of the shortest path from <math|v> to
  <math|w> in <math|G>.

  The lengths/weights of the shortest paths are obtained by the command
  <verbatim|<hlink|allpairs_distance|#allpairs-distance>> if <math|G> is
  weighted resp.<nbsp>the command <verbatim|<hlink|vertex_distance|#vertex-distance>>
  if <math|G> is unweighted. Therefore <math|T> is constructed in at most
  <math|O<around*|(|<around*|\||V|\|><rsup|3>|)>> time if
  <verbatim|weighted[=true]> is given and in
  <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>> time otherwise.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph([1,2,3,4,5,6],%{[1,2],[2,3],[2,4],[4,5],[3,5]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 6 vertices and 5 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/digraph3.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=transitive_closure(G,weighted)
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 6 vertices and 9 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T)
    </input>
  </session>

  <center|<image|images/digraph4.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=assign_edge_weights(G,1,99)
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 6 vertices and 5 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/digraph5.eps|35%|||>>

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(transitive_closure(G,weighted=true))
    </input>
  </session>

  <center|<image|images/digraph6.eps|35%|||>>

  <subsection|Line graph>

  The command <verbatim|line_graph><index|<verbatim|line_graph>> is used for
  construction of <hlink|<rigid|line graphs>|https://en.wikipedia.org/wiki/Line_graph>
  <cite-detail|godsil|pp.<nbsp>10>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|line_graph(G)>>>>>>

  <verbatim|line_graph> takes an undirected graph <math|G> as its only
  argument and returns the line graph <math|L<around*|(|G|)>> with
  <math|<around*|\||E|\|>> distinct vertices, one vertex for each edge in
  <math|E>. Furthermore, two vertices <math|v<rsub|1>> and <math|v<rsub|2>>
  in <math|L<around*|(|G|)>> are adjacent if and only if the corresponding
  edges <math|e<rsub|1>,e<rsub|2>\<in\>E> have a common endpoint. The
  vertices in <math|L<around*|(|G|)>> are labeled with strings in form
  <verbatim|v-w>, where <math|e=v w\<in\>E>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K4:=complete_graph([1,2,3,4])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L:=line_graph(K4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(L,spring)
    </input>
  </session>

  <center|<image|images/linegraph.eps|35%|||>>

  <subsection|Plane dual graph>

  The command <verbatim|plane_dual><index|<verbatim|plane_dual>> is used for
  construction of <hlink|<rigid|dual graphs>|https://en.wikipedia.org/wiki/Dual_graph>
  from undirected biconnected <hlink|<rigid|planar
  graphs>|https://en.wikipedia.org/wiki/Planar_graph>. To determine whether a
  graph is planar <cite-detail|godsil|pp.<nbsp>12> use the command
  <verbatim|is_planar><index|<verbatim|is_planar>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|plane_dual(G)>>>|<row|<cell|>|<cell|<verbatim|plane_dual(F)>>>|<row|<cell|>|<cell|<verbatim|is_planar(G)>>>|<row|<cell|>|<cell|<verbatim|is_planar(G,F)>>>>>>

  <verbatim|plane_dual> takes a biconnected planar graph
  <math|G<around*|(|V,E|)>> or the list <math|F> of faces of a planar
  embedding of <math|G> as its only argument and returns the graph <math|H>
  with faces of <math|G> as its vertices. Two vertices in <math|H> are
  adjacent if and only if the corresponding faces share an edge in <math|G>.
  The algorithm runs in <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> time.

  Note that the concept of dual graph is normally defined for multigraphs. By
  the strict definition, every planar multigraph has the corresponding dual
  multigraph; moreover, the dual of the latter is equal to the former. Since
  <samp|Giac> generally does not support multigraphs, a simplified definition
  suitable for simple graphs is used; hence the requirement that the input
  graph is biconnected.

  In the example below, the dual graph of the cube graph is obtained.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=hypercube_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,spring)
    </input>
  </session>

  <center|<image|images/cube.eps|35%|||>>

  The cube has six faces, hence its plane dual graph <math|D> has six
  vertices. Also, every face obviously shares an edge with exactly four other
  faces, so the degree of each vertex in <math|D> is equal to 4.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D:=plane_dual(H)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D,spring)
    </input>
  </session>

  <center|<image|images/dual.eps|35%|||>>

  <verbatim|is_planar> takes one or two arguments, the input graph <math|G>
  and optionally an unassigned identifier<nbsp><verbatim|F>. It returns
  <verbatim|true> if <math|G> is planar and <verbatim|false> otherwise. If
  the second argument is given and <math|G> is planar and biconnected, the
  list of faces of <math|G> is stored to <verbatim|F>. Each face is
  represented as a cycle (a list) of vertices. The strategy is to use the
  algorithm of <name|Demoucron> et al.<nbsp><cite-detail|gibbons|pp.<nbsp>88>,
  which runs in <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_planar(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_planar(graph("durer"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  In the next example, a graph isomorphic to <math|D> is obtained when
  passing a list of faces of <math|H> to <verbatim|plane_dual>. The order of
  vertices is determined by the order of faces.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_planar(H,F); F
    <|unfolded-io>
      <\equation*>
        <text|true>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|4|4|cell-rborder|0ln>|<table|<row|<cell|<with|math-font-family|rm|010>>|<cell|<with|math-font-family|rm|000>>|<cell|<with|math-font-family|rm|001>>|<cell|<with|math-font-family|rm|011>>>|<row|<cell|<with|math-font-family|rm|001>>|<cell|<with|math-font-family|rm|000>>|<cell|<with|math-font-family|rm|100>>|<cell|<with|math-font-family|rm|101>>>|<row|<cell|<with|math-font-family|rm|010>>|<cell|<with|math-font-family|rm|011>>|<cell|<with|math-font-family|rm|111>>|<cell|<with|math-font-family|rm|110>>>|<row|<cell|<with|math-font-family|rm|100>>|<cell|<with|math-font-family|rm|000>>|<cell|<with|math-font-family|rm|010>>|<cell|<with|math-font-family|rm|110>>>|<row|<cell|<with|math-font-family|rm|111>>|<cell|<with|math-font-family|rm|011>>|<cell|<with|math-font-family|rm|001>>|<cell|<with|math-font-family|rm|101>>>|<row|<cell|<with|math-font-family|rm|101>>|<cell|<with|math-font-family|rm|100>>|<cell|<with|math-font-family|rm|110>>|<cell|<with|math-font-family|rm|111>>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(plane_dual(F),D)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Truncating planar graphs>

  The command <verbatim|truncate_graph><index|<verbatim|truncate_graph>>
  performs <hlink|truncation|https://en.wikipedia.org/wiki/Truncation_(geometry)>
  of biconnected planar graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|truncate_graph(G)>>>>>>

  <verbatim|truncate_graph> takes a biconnected planar graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the graph
  obtained by truncating the respective polyhedron, i.e.<nbsp>by \Pcutting
  off\Q its vertices. The resulting graph has
  <math|2*\|E<around*|\|||\<nobracket\>>> vertices and
  <math|3*<around*|\||E|\|>> edges. The procedure of truncating a graph by
  subdividing its edges is described in<nbsp><cite|andova>.

  The algorithm requires computing a planar embedding of <math|G>, which is
  done by applying <name|Demoucron>'s algorithm. Hence its complexity is
  <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=truncate_graph(graph("dodecahedron"))
    <|unfolded-io>
      <\equation*>
        an undirected unweighted graph with 60 vertices and 90 edges
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar,labels=false)
    </input>
  </session>

  <center|<image|images/trunc.eps|35%|||>>

  Truncating the plane dual of <math|G> represents the <strong|leapfrog
  operation> on <math|G>, which can be used for constructing
  <hlink|<rigid|fullerene graphs>|https://en.wikipedia.org/wiki/Fullerene#Other_buckyballs><nbsp><cite|andova>.
  By performing the leapfrog operation on a fullerene graph one obtains a
  larger fullerene. For example, the dual of the Errera graph is a fullerene
  (see <hlink|here|https://en.wikipedia.org/wiki/Errera_graph#Applications_in_chemistry>);
  hence by truncating Errera graph (i.e.<nbsp>the dual of its dual) one
  obtains a fullerene.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=truncate_graph(graph("errera"))
    <|unfolded-io>
      <\equation*>
        an undirected unweighted graph with 90 vertices and 135 edges
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_planar(G,F)
    <|unfolded-io>
      <\equation*>
        true
      </equation*>
    </unfolded-io>
  </session>

  Now <math|F> contains a list of faces of the graph <math|G>. Since <math|G>
  is a fullerene, every face is a 5- or 6-cycle.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      set[op(apply(length,F))]
    <|unfolded-io>
      <\equation*>
        set[5,6]
      </equation*>
    </unfolded-io>
  </session>

  When drawing fullerenes, it is recommended to use the
  <hlink|<rigid|circular method>|#circle-method> since it usually produces
  best results. Any face of the planar embedding of a given fullerene be
  chosen as the outer face, as in the example below.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle=rand(F))
    </input>
  </session>

  <center|<image|images/fullerene.eps|35%|||>>

  <section|Random graphs>

  <subsection|Random general graphs><label|random-graph>

  The commands <verbatim|random_graph><index|<verbatim|random_graph>> and
  <verbatim|random_digraph><index|<verbatim|random_digraph>> are used for
  generating general (di)graphs at random according to various models,
  including <hlink|<rigid|preferential attachment>|https://en.wikipedia.org/wiki/Preferential_attachment>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|1|3|3|cell-valign|c>|<cwith|1|-1|3|3|cell-lsep|1em>|<cwith|5|5|3|3|cell-valign|c>|<cwith|7|7|3|3|cell-valign|c>|<cwith|5|5|3|3|cell-row-span|2>|<cwith|5|5|3|3|cell-col-span|1>|<cwith|1|1|3|3|cell-row-span|4>|<cwith|1|1|3|3|cell-col-span|1>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_graph(n\|L,p)>>|<cell|<very-small|Erd®s\URényi
  model>>>|<row|<cell|>|<cell|<verbatim|random_graph(n\|L,m)>>|<cell|>>|<row|<cell|>|<cell|<verbatim|random_digraph(n\|L,p)>>|<cell|>>|<row|<cell|>|<cell|<verbatim|random_digraph(n\|L,m)>>|<cell|>>|<row|<cell|>|<cell|<verbatim|random_graph(n\|L,[p0,p1,...])>>|<cell|<very-small|custom
  vertex degree distribution>>>|<row|<cell|>|<cell|<verbatim|random_graph(n\|L,f)>>|<cell|>>|<row|<cell|>|<cell|<verbatim|random_graph(n\|L,d,k)>>|<cell|<very-small|preferential
  attachment>>>>>>

  <verbatim|random_graph> and <verbatim|random_digraph> can both take two
  arguments: a positive integer <math|n> or a list of labels <math|L> of
  length <math|n>. The second argument is a positive real number
  <math|p\<less\>1> or a positive integer <math|m>. The return value is a
  (di)graph on <math|n> vertices (with elements of <math|L> as vertex labels)
  selected uniformly at random, i.e.<nbsp>a (di)graph in which each edge/arc
  is present with probability <math|p> or which contains exactly <math|m>
  edges/arcs chosen uniformly at random (<hlink|Erd®s\URényi|https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model>
  model).

  Erd®s\URényi model is implemented according to <name|Bagatelj> and
  <name|Brandes><nbsp><cite-detail|bagatelj|algorithms<nbsp>1<nbsp>and<nbsp>2>.
  The corresponding algorithms run in linear time and are suitable for
  generating large graphs.

  <verbatim|random_graph> can also generate graphs with respect to a given
  probability distribution of vertex degrees if the second argument is a
  discrete probability density function given as a list of probabilities or
  weights <math|<around*|(|p<rsub|0>,p<rsub|1>,\<ldots\>,p<rsub|n-1>|)>> or
  as a weight function <math|f\<of\>\<bbb-N\>\<cup\><around*|{|0|}>\<rightarrow\><around*|[|0,+\<infty\>|\<rangle\>>>
  such that <math|f<around*|(|i|)>=p<rsub|i>> for <math|i=0,1,\<ldots\>,n-1>.
  Trailing zeros in the list of weights, if present, may be omitted. The
  numbers <math|p<rsub|i>> are automatically scaled by
  <math|1/<big|sum><rsub|i=1><rsup|n-1>p<rsub|i>> to achieve the sum of 1 and
  a graph with that precise distribution of vertex degrees is generated at
  random using the algorithm described in<nbsp><cite-detail|newman|pp.<nbsp>2567>
  with some modifications. First, a degree sequence <math|d> is generated
  randomly by drawing samples from the given distribution and repeating the
  process until a graphic sequence is obtained. Then the algorithm for
  constructing a feasible solution from <math|d><nbsp><cite|hakimi> is
  applied. Finally, the edges of that graph are randomized by choosing
  suitable pairs of nonincident edges and \Prewiring\Q them without changing
  the degree sequence. Two edges <math|u v> and <math|w z> can be rewired in
  at most two ways, becoming either <math|u z> and <math|w v> or <math|u w>
  and <math|v z> (if these edges are not in the graph already). Letting
  <math|m> denote the number of edges, at most

  <\equation*>
    N=<around*|\<lceil\>|<around*|(|log<rsub|2>
    <frac|m|m-1>|)><rsup|-1>|\<rceil\>>\<less\>m
  </equation*>

  such choices is made, assuring that the probability of rewiring each edge
  at least once is larger than <math|<frac|1|2>>. The total complexity of
  this algorithm is <math|O<around*|(|n<rsup|2>*log n|)>>.

  Additionally, to support generation of realistic networks,
  <verbatim|random_graph> can be used with integer parameters
  <math|d\<gtr\>0> and <math|k\<geqslant\>0> as the second and the third
  argument, respectively, in which case a preferential attachment rule is
  applied in the following way. For <math|n\<geqslant\>2>, the resulting
  graph <math|G<around*|(|V,E|)>> initially contains two vertices
  <math|v<rsub|1>,v<rsub|2>> and one edge <math|v<rsub|1> v<rsub|2>>. For
  each <math|i=3,\<ldots\>,n>, the vertex <math|v<rsub|i>> is added to
  <math|V> along with edges <math|v<rsub|i> v<rsub|j>> for
  <math|min<around*|{|i-1,d|}>> mutually different values of <math|j>, which
  are chosen at random in the set <math|<around*|{|1,2,\<ldots\>,i-1|}>> with
  probability

  <\equation*>
    p<rsub|j>=<frac|deg v<rsub|j>|<big|sum><rsub|r=1><rsup|i-1>deg
    v<rsub|r>>.
  </equation*>

  Subsequently, additional at most <math|k> random edges connecting the
  neighbors of <math|v<rsub|i>> to each other are added to <math|E>, allowing
  the user to control the <hlink|<rigid|clustering
  coefficient>|#clustering-coefficient> of <math|G>. This method is due to
  <name|Schank> and <name|Wagner><nbsp><cite-detail|schank|Algorithm<nbsp>2,
  pp.<nbsp>271>. The time complexity of the implementation is
  <math|O<around*|(|n<rsup|2>*d+n*k|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 21 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <\center>
    <image|images/rand7.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(1000,0.05)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1000 vertices and 24870
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      minimum_degree(G),maximum_degree(G)
    <|unfolded-io>
      <\equation*>
        20,71
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(15,20)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <\center>
    <image|images/rand8.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      DG:=random_digraph(15,0.15)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 15 vertices and 33 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(DG,labels=false,spring)
    </input>
  </session>

  <\center>
    <image|images/rand10.eps|35%|||>
  </center>

  In the following example, a random graph is generated such that the degree
  of each vertex is drawn from <math|<around*|{|0,1,\<ldots\>,10|}>>
  according to weights specified in the table below.

  <center|<block|<tformat|<table|<row|<cell|degree>|<cell|0>|<cell|1>|<cell|2>|<cell|3>|<cell|4>|<cell|5>|<cell|6>|<cell|7>|<cell|8>|<cell|9>|<cell|10>>|<row|<cell|weight>|<cell|0>|<cell|0>|<cell|9>|<cell|7>|<cell|0>|<cell|5>|<cell|4>|<cell|3>|<cell|0>|<cell|1>|<cell|1>>>>>>

  That is, the degrees are generated with probabilities
  <math|0,0,<frac|3|10>,<frac|7|30>,0,<frac|1|6>,<frac|2|15>,<frac|1|10>,0,<frac|1|30>,<frac|1|30>>,
  respectively.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10000,[0,0,9,7,0,5,4,3,0,1,1])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 21231
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      frequencies(degree_sequence(G))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|0.3125>>|<row|<cell|3>|<cell|0.2256>>|<row|<cell|5>|<cell|0.163>>|<row|<cell|6>|<cell|0.1331>>|<row|<cell|7>|<cell|0.0987>>|<row|<cell|9>|<cell|0.0311>>|<row|<cell|10>|<cell|0.036>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  In the example below, a random graph is generated such that the vertex
  degrees are distributed according to the following weight function:

  <\equation*>
    f<around*|(|k|)>=<choice|<tformat|<table|<row|<cell|0,>|<cell|k=0,>>|<row|<cell|k<rsup|-3/2>*\<mathe\><rsup|-k/3>,>|<cell|k\<geqslant\>1.>>>>>
  </equation*>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10000,k-\<gtr\>when(k\<less\>1,0,k^-1.5*exp(-k/3)))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 8017
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      length(connected_components(G))
    <|unfolded-io>
      <\equation*>
        2266
      </equation*>
    </unfolded-io>
  </session>

  The command line below computes the average size of a connected component
  in <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      round(mean(apply(length,connected_components(G))))
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>
  </session>

  The next example demonstrates how to generate random graphs with adjustable
  clustering coefficient.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=random_graph(10000,5,10)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 105628
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G1)
    <|unfolded-io>
      <\equation*>
        0.469236344448
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=random_graph(10000,5,20)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 121957
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G2)
    <|unfolded-io>
      <\equation*>
        0.612673551668
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G3:=random_graph(10000,10,5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 143646
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G3)
    <|unfolded-io>
      <\equation*>
        0.113671512462
      </equation*>
    </unfolded-io>
  </session>

  The distribution of vertex degrees in a graph generated with preferential
  attachment rule roughly obeys the power law in its tail, as shown in the
  example below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10000,5,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 67875
        edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      histogram(degree_sequence(G))
    </input>
  </session>

  <\center>
    <image|images/powlaw.eps|35%|||>
  </center>

  <subsection|Random bipartite graphs>

  The command <verbatim|random_bipartite_graph><index|<verbatim|random_bipartite_graph>>
  is used for generating <hlink|<rigid|bipartite graphs>|#is-bipartite> at
  random.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_bipartite_graph(n,p\|m)>>>|<row|<cell|>|<cell|<verbatim|random_bipartite_graph([a,b],p\|m)>>>>>>

  <verbatim|random_bipartite_graph> takes two arguments. The first argument
  is either a positive integer <math|n> or a list of two positive integers
  <math|a> and <math|b>. The second argument is either a positive real number
  <math|p\<less\>1> or a positive integer <math|m>. The command returns a
  random bipartite graph on <math|n> vertices (or with two partitions of
  sizes <math|a> and <math|b>) in which each possible edge is present with
  probability <math|p> (or <math|m> edges are inserted at random).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_bipartite_graph([3,4],0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/rand6.eps|35%|||>>

  \;

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_bipartite_graph(30,60)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 60 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Random trees>

  The command <verbatim|random_tree><index|<verbatim|random_tree>> is used
  for generating <hlink|tree graphs|#is-tree> at random.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|1|3|3|cell-lsep|1em>|<cwith|2|-1|3|3|cell-lsep|1em>|<cwith|3|3|3|3|cell-row-span|2>|<cwith|3|3|3|3|cell-col-span|1>|<cwith|3|3|3|3|cell-valign|c>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_tree(n\|V)>>|<cell|<very-small|unrooted
  unlabeled trees>>>|<row|<cell|>|<cell|<verbatim|random_tree(n\|V,d)>>|<cell|<very-small|trees
  with limited maximum degree>>>|<row|<cell|>|<cell|<verbatim|random_tree(n\|V,root)>>|<cell|<very-small|rooted
  unlabeled trees>>>|<row|<cell|>|<cell|<verbatim|random_tree(V,root=v)>>|<cell|>>>>>

  <verbatim|random_tree> takes one or two arguments: a positive integer
  <math|n> or a list <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>
  and optionally an integer <math|d\<geqslant\>2> or the option
  <verbatim|root[=v]>, where <math|v\<in\>V>. It returns a random tree
  <math|T<around*|(|V,E|)>> on <math|n> vertices such that

  <\itemize-minus>
    <item>if the second argument is omitted, then <math|T> is uniformly
    selected among all unrooted unlabeled trees on <math|n> vertices,

    <item>if <math|d> is given as the second argument, then
    <math|\<Delta\><around*|(|T|)>\<leqslant\>d>, where
    <math|\<Delta\><around*|(|T|)>> is the maximum vertex degree in <math|T>,

    <item>if <verbatim|root[=v]> is given as the second argument, then
    <math|T> is uniformly selected among all rooted unlabeled trees on
    <math|n> vertices. If <math|v> is specified then the vertex labels in
    <math|V> (required) will be assigned to vertices in <math|T> such that
    <math|v> is the first vertex in the list returned by the command
    <verbatim|vertices>.
  </itemize-minus>

  Rooted unlabeled trees are generated uniformly at random using
  <abbr|RANRUT> algorithm <cite-detail|nijenhuis|pp.<nbsp>274>. The root of a
  tree <math|T> generated this way, if not specified as <math|v>, is always
  the first vertex in the list returned by <verbatim|vertices>. The average
  time complexity of <abbr|RANRUT> algorithm is <math|O<around*|(|n*log
  n|)>><nbsp><cite|alonso>.

  Unrooted unlabeled trees, also called <strong|free> trees, are generated
  uniformly at random using <name|Wilf>'s algorithm<\footnote>
    The original <name|Wilf>'s algorithm has a minor flaw in the procedure
    <verbatim|Free><nbsp><cite-detail|wilf|pp.<nbsp>207>. In the formula
    <math|p=<binom|1+a<rsub|n/2>|2>/a<rsub|n>> in step<nbsp>(T1) the
    denominator <math|a<rsub|n>> stands for the number of all rooted
    unlabeled trees on <math|n> vertices. However, one should divide by the
    number <math|t<rsub|n>> of all <em|unrooted> unlabeled trees instead,
    which can be obtained from <math|a<rsub|1>,a<rsub|2>,\<ldots\>,a<rsub|n>>
    by applying the formula in <cite-detail|otter|pp.<nbsp>589>. This
    implementation includes the correction.
  </footnote><nbsp><cite|wilf>, which is based on <abbr|RANRUT> algorithm and
  runs in about the same time as <abbr|RANRUT> itself.

  Trees with bounded maximum degree are generated using a simple algorithm
  which starts with an empty tree and adds edges at random one at a time. It
  is much faster than <abbr|RANRUT> but selects trees in a non-uniform
  manner. To force the use of this algorithm even without vertex degree limit
  (for example, when <math|n> is very large), one can set
  <math|d=+\<infty\>>.

  For example, the command line below creates a forest containing 10 randomly
  selected free trees on 10 vertices.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=disjoint_union(apply(random_tree,[10$10]))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 90 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,tree,labels=false)
    </input>
  </session>

  <\center>
    <image|images/rand5.eps|35%|||>
  </center>

  The following example demonstrates the uniformity of random generation of
  free trees. Letting <math|n=6>, there are exactly 6 distinct free trees on
  6 vertices, created by the next command line.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      trees:=[star_graph(5),path_graph(6),graph(trail(1,2,3,4),trail(5,4,6)),
      graph(%{[1,2],[2,3],[2,4],[4,5],[4,6]%}),graph(trail(1,2,3,4),trail(3,5,6)),graph(trail(1,2,3,4),trail(5,3,6))]:;
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(disjoint_union(trees),spring,labels=false)
    </input>
  </session>

  <\center>
    <image|images/trees6.eps|35%|||>
  </center>

  Now, generating a random free tree on <math|6> nodes always produces one of
  the above six graphs, which is determined by using the command
  <hlink|<verbatim|is_isomorphic>|#is-isomorphic>. 1200 trees are generated
  in total and the number of occurrences of <verbatim|trees[k]> is stored in
  <verbatim|hits[k]> for every <math|k=1,2,\<ldots\>,6> (note that in
  <samp|Xcas> mode it is actually <math|k=0,\<ldots\>,5>).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      hits:=[0$6]:;

      for k from 1 to 1200 do

      \ \ \ \ T:=random_tree(6);

      \ \ \ \ for j from 0 to 5 do

      \ \ \ \ \ \ \ \ if is_isomorphic(T,trees[j]) then hits[j]++; fi;

      \ \ \ \ od;

      od:;

      hits
    <|unfolded-io>
      <\equation*>
        <around|[|198,194,192,199,211,206|]>
      </equation*>
    </unfolded-io>
  </session>

  To show that the algorithm also selects rooted trees on <math|n> vertices
  with equal probability, one can reproduce the example
  in<nbsp><cite-detail|nijenhuis|pp.<nbsp>281>, in which <math|n=5>. First,
  all distinct rooted trees on 5 vertices are created and stored in
  <verbatim|trees>; there are exactly nine of them. Their root vertices are
  highlighted to be distinguishable. Then, 4500 rooted trees on 5 vertices
  are generated at random, highlighting the root vertex in each of them. As
  in the previous example, the variable <verbatim|hits[k]> records how many
  of them are isomorphic to <verbatim|trees[k]>.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      trees:=[

      highlight_vertex(graph(trail(1,2,3,4,5)),1),

      highlight_vertex(graph(trail(1,2,3,4,5)),2),

      highlight_vertex(graph(trail(1,2,3,4,5)),3),

      highlight_vertex(graph(trail(1,2,3),trail(4,3,5)),1),

      highlight_vertex(graph(trail(1,2,3),trail(4,3,5)),2),

      highlight_vertex(graph(trail(1,2,3),trail(4,3,5)),3),

      highlight_vertex(graph(trail(1,2,3),trail(4,3,5)),4),

      highlight_vertex(graph(trail(1,2,3),trail(4,2,5)),1),

      highlight_vertex(graph(trail(1,2,3),trail(4,2,5)),2)

      ]:;
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(disjoint_union(trees),labels=false)
    </input>
  </session>

  <center|<image|images/ninetrees.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      hits:=[0$9]:;

      for k from 1 to 4500 do

      \ \ \ \ T:=random_tree(5,root);

      \ \ \ \ HT:=highlight_vertex(T,vertices(T)[0]);

      \ \ \ \ for j from 0 to 8 do

      \ \ \ \ \ \ \ \ if is_isomorphic(HT,trees[j]) then hits[j]++; fi;

      \ \ \ \ od;

      od:;

      hits
    <|unfolded-io>
      <\equation*>
        <around|[|534,483,486,485,496,521,498,489,508|]>
      </equation*>
    </unfolded-io>
  </session>

  In the following example, a random tree on 100 vertices with maximum degree
  at most 3 is drawn.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(random_tree(100,3))
    </input>
  </session>

  <center|<image|images/rtree.eps|35%|||>>

  <subsection|Random planar graphs><label|random-planar>

  The command <verbatim|random_planar_graph><index|<verbatim|random_planar_graph>>
  is used for generating random planar graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_planar_graph(n\|L,p)>>>|<row|<cell|>|<cell|<verbatim|random_planar_graph(n\|L,p,k)>>>>>>

  <verbatim|random_planar_graph> takes two or three arguments, a positive
  integer <math|n> (or a list <math|L> of length <math|n>), a positive real
  number <math|p\<less\>1> and optionally an integer
  <math|k\<in\><around*|{|0,1,2,3|}>> (by default, <math|k=1>). The command
  returns a random <math|k>-connected planar graph on <math|n> vertices
  (using the elements of <math|L> as vertex labels).

  The result is obtained by first generating a random maximal planar graph
  and then attempting to remove each edge with probability <math|p>,
  maintaining the <math|k>-connectivity of the graph (if <math|k=0>, the
  graph may be disconnected). The running time is <math|O<around*|(|n|)>> if
  <math|k=0>, <math|O<around*|(|n<rsup|2>|)>> if
  <math|k\<in\><around*|{|1,2|}>> and <math|O<around*|(|n<rsup|3>|)>> if
  <math|k=3>.

  The following command line generates a biconnected planar graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(20,0.8,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 25 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar,labels=false)
    </input>
  </session>

  <center|<image|images/rand1.eps|35%|||>>

  The command line below generates a triconnected planar graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(15,0.9,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 25 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar,labels=false)
    </input>
  </session>

  <center|<image|images/rand2.eps|35%|||>>

  The next command line generates a disconnected planar graph with high
  probability.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(30,0.9,0)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 23 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_forest(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/rand3.eps|35%|||>>

  By default, a connected planar graph is generated, like in the following
  example.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(15,0.618)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 19 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      articulation_points(G)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,4,10,11|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar)
    </input>
  </session>

  <center|<image|images/rand4.eps|35%|||>>

  <subsection|Random graphs from a given degree sequence>

  The command <verbatim|random_sequence_graph><index|<verbatim|random_sequence_graph>>
  is used for generating a random undirected graph with a specified degree
  sequence.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_sequence_graph(L)>>>>>>

  <verbatim|random_sequence_graph> takes the degree sequence <math|L> (a list
  of nonnegative integers) as its only argument. It returns an asimptotically
  uniform random graph with the degree sequence equal to <math|L> using the
  algorithm developed by <name|Bayati> et al.<nbsp><cite|bayati>.

  The algorithm slows down quickly and uses
  <math|O<around*|(|<around*|\||L|\|><rsup|2>|)>> of auxiliary space, so it
  is best used for a few hundreds of vertices or less.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      s:=[1,3,3,2,1,2,2,2,3,3]:; is_graphic_sequence(s)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_sequence_graph(s)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 11 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/seq.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=random_sequence_graph(s)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 11 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,spring)
    </input>
  </session>

  <center|<image|images/seq2.eps|35%|||>>

  <subsection|Random regular graphs>

  The command <verbatim|random_regular_graph><index|<verbatim|random_regular_graph>>
  is used for generating random <hlink|<rigid|regular graphs>|#is-regular> on
  a specified set of vertices.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_regular_graph(n
  or L,d)>>>|<row|<cell|>|<cell|<verbatim|random_regular_graph(n or
  L,d,connected)>>>>>>

  <verbatim|random_regular_graph> takes two mandatory arguments, a positive
  integer <math|n> (or a list <math|L> of length <math|n>) and a nonnegative
  integer <math|d>. Optionally, the option <verbatim|connected> may be
  specified as a third argument, indicating that the generated graph must be
  connected. The command creates <math|n> vertices (using elements of
  <math|L> as vertex labels) and returns a random <math|d>-regular
  (connected) graph on these vertices.

  Note that a <math|d>-regular graph on <math|n> vertices exists if and only
  if <math|n\<gtr\>d+1> and <math|n*d> is even. If these conditions are not
  met, <verbatim|random_regular_graph> returns an error.

  The strategy is to use the algorithm developed by <name|Steger> and
  <name|Wormald><nbsp><cite-detail|steger|algorithm<nbsp>2>. The runtime is
  negligible for <math|n\<leqslant\>100>. However, for <math|n\<gtr\>200> the
  algorithm is considerably slower. Graphs are generated with approximately
  uniform probability, which means that for <math|n\<rightarrow\>\<infty\>>
  and <math|d> not growing so quickly with <math|n> the probability
  distribution converges to uniformity.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_regular_graph(16,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 24 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      degree_sequence(G)
    <|unfolded-io>
      <\equation*>
        <around|[|3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/regular.eps|35%|||>>

  <subsection|Random tournaments>

  The command <verbatim|random_tournament><index|<verbatim|random_tournament>>
  is used for generating random <hlink|tournaments|#is-tournament>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_tournament(n)>>>|<row|<cell|>|<cell|<verbatim|random_tournament(L)>>>>>>

  <verbatim|random_tournament> takes a positive integer <math|n> or a list
  <math|L> of length <math|n> as its only argument and returns a random
  tournament on <math|n> vertices. If <math|L> is specified, its elements are
  used to label the vertices.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tournament([1,2,3,4])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tournament(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/tnm.eps|35%|||>>

  <subsection|Random network graphs><label|random-network>

  The command <verbatim|random_network><index|<verbatim|random_network>> is
  used for generation of random <hlink|networks|#is-network>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|random_network(a,b,[opts])>>>|<row|<cell|>|<cell|<verbatim|random_network(a,b,p,[opts])>>>>>>

  <verbatim|random_network> takes two to four arguments: a positive integer
  <math|a>, a positive integer <math|b>, an optional real number <math|p>
  such that <math|0\<less\>p\<leqslant\>1> (by default <math|p=0.5>) and
  optionally a sequence of options <verbatim|opts>. The supported options are
  <verbatim|acyclic[=true\|false]> and <verbatim|weights=a..b>.

  The command returns a network graph with <math|a<rsup|2>*b> vertices which
  is composed as follows (the method of generating the network skeleton is
  due to <name|Goldfarb> and <name|Grigoriadis><nbsp><cite|goldfarb>).

  Firstly, grid graphs <math|F<rsub|1>,F<rsub|2>,\<ldots\>,F<rsub|b>> (called
  <strong|frames>), each of them with <math|a\<times\>a> vertices, are
  generated. If the option <verbatim|acyclic[=true]> is used (by default is
  <verbatim|acyclic=false>), then an acyclic orientation is computed for each
  frame using <samp|st>-ordering (see Section<nbsp><reference|st-ordering>)
  with two opposite corners of the frame as source and sink, otherwise all
  vertices in the frame are connected to their neighbors (forth and back). In
  addition, for each <math|k\<less\>b> the vertices of <math|F<rsub|k>> are
  connected one to one with the vertices of the next frame <math|F<rsub|k+1>>
  using a random permutation of those vertices. The first vertex of the first
  frame is the source and the last vertex of the last frame is the sink of
  the network (some arcs may have to be removed to achieve that). Finally,
  the removal of each arc is attempted with probability <math|1-p> (unless
  its removal disconnects the network), making each arc present with
  probability <math|p>.

  if the option <verbatim|weights=a..b> is specified, arc weights in the
  network are randomized in the interval <math|<around*|[|a,b|]>\<subset\>\<bbb-R\>>.
  If <math|a,b> are integers, the weights are also integers.

  For example, the command line below creates a random network, consisting of
  3 frames of size <math|2\<times\>2>, in which each arc is present with the
  probability 0.9.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N1:=random_network(2,3,0.9)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 12 vertices and 25 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N1,spring)
    </input>
  </session>

  <\center>
    <image|images/network1.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N1)
    <|unfolded-io>
      <\equation*>
        <around|[|0|]>,<around|[|11|]>
      </equation*>
    </unfolded-io>
  </session>

  In the next example, passing the option <verbatim|acyclic> forces the
  output graph to be acyclic.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N2:=random_network(3,2,0.618,acyclic)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 18 vertices and 22 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N2,spring)
    </input>
  </session>

  <\center>
    <image|images/network2.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N2)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|10>>|<row|<cell|4>|<cell|17>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  Arc weights can be randomized, as demonstrated below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N3:=random_network(2,2,0.75,weights=1..9)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 8 vertices and 12 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N3,spring)
    </input>
  </session>

  <center|<image|images/networkw.eps|35%|||>>

  <subsection|Randomizing edge weights><label|assign-edge-weights>

  The command <verbatim|assign_edge_weights><index|<verbatim|assign_edge_weights>>
  is used for assigning weights to edges of graphs at random.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|assign_edge_weights(G,a..b)>>>|<row|<cell|>|<cell|<verbatim|assign_edge_weights(G,m,n)>>>>>>

  <verbatim|assign_edge_weights> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>> and an interval <math|a><nbsp>..<nbsp><math|b> of
  real numbers or a sequence of two positive integers <math|m>
  and<nbsp><math|n>. The command operates such that for, each edge
  <math|e\<in\>E>, the weight of <math|e> is chosen uniformly from the real
  interval <math|<around*|[|a,b|)>> or from the set of integers lying between
  <math|m> and <math|n>, including both <math|m> and <math|n>. After
  assigning weights to all edges, a modified copy of <math|G> is returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=assign_edge_weights(grid_graph(5,3),1,99)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 15 vertices and 22 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/grid.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=assign_edge_weights(graph_complement(complete_graph(2,3,4)),e..pi)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 9 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/rand9.eps|35%|||>>

  <chapter|Modifying graphs>

  <section|Promoting to directed and weighted graphs>

  <subsection|Converting edges to arcs><label|make-directed>

  To promote an existing undirected graph to a directed one, use the command
  <kbd|make_directed><index|<kbd|make_directed>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|make_directed(G)>>>|<row|<cell|>|<cell|<verbatim|make_directed(G,A)>>>>>>

  <with|font-family|tt|make_directed> is called with one or two arguments, an
  undirected graph <math|G<around|(|V,E|)>> and optionally a numerical square
  matrix <math|A=<around*|[|a<rsub|i j>|]>> of order <math|<around|\||V|\|>>.
  Every edge <math|<around|{|i,j|}>\<in\>E> is replaced with the pair of arcs
  <math|<around|(|i,j|)>> and <math|<around|(|j,i|)>> and, if matrix <math|A>
  is specified, its elements <math|a<rsub|i*j>> and <math|a<rsub|j*i>> are
  assigned as weights of these arcs, respectively. Thus a directed (weighted)
  copy of <math|G> is constructed and subsequently returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      make_directed(cycle_graph(4))
    <|unfolded-io>
      <\equation*>
        <text|C4: a directed unweighted graph with 4 vertices and 8 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      make_directed(cycle_graph(4),[[0,0,0,1],[2,0,1,3],[0,1,0,4],[5,0,4,0]])
    <|unfolded-io>
      <\equation*>
        <text|C4: a directed weighted graph with 4 vertices and 8 arcs>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Assigning weight matrix to unweighted
  graphs><label|make-weighted>

  To promote an existing unweighted graph to a weighted one, use the command
  <kbd|make_weighted><index|<kbd|make_weighted>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|make_weighted(G)>>>|<row|<cell|>|<cell|<verbatim|make_weighted(G,A)>>>>>>

  <with|font-family|tt|make_weighted> takes one or two arguments, an
  unweighted graph <math|G<around|(|V,E|)>> and optionally a square matrix
  <math|A=<around*|[|a<rsub|i j>|]>> of order <math|<around|\||V|\|>>. If the
  matrix specification is omitted, a square matrix of ones is assumed. Then a
  copy of <math|G> is returned in which each edge/arc
  <math|<around|(|i,j|)>\<in\>E> gets the element <math|a<rsub|i*j>> in
  <math|A> assigned as its weight. If <math|G> is undirected, it is assumed
  that <math|A> is a symmetric matrix.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      make_weighted(graph(%{[1,2],[2,3],[3,1]%}),[[0,2,3],[2,0,1],[3,1,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Modifying vertices of a graph>

  <subsection|Adding and removing vertices>

  For adding and removing vertices to/from graphs use the commands
  <kbd|add_vertex><index|<kbd|add_vertex>> and
  <kbd|delete_vertex><index|<kbd|delete_vertex>>, respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|add_vertex(G,v\|L)>>>|<row|<cell|>|<cell|<verbatim|delete_vertex(G,v\|L)>>>>>>

  The command <with|font-family|tt|add_vertex> takes two arguments, a graph
  <math|G<around|(|V,E|)>> and a single label <math|v> or a list of labels
  <math|L>, and returns the graph <math|G<rprime|'>*<around|(|V\<cup\><around|{|v|}>,E|)>>
  or <math|G<rprime|''>*<around|(|V\<cup\>L,E|)>> if a list <math|L> is
  given.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_vertex(K5,6)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_vertex(K5,[a,b,c])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 10 edges>
      </equation*>
    </unfolded-io>
  </session>

  Note that vertices already present in <math|G> will not be added. For
  example:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_vertex(K5,[4,5,6])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 10 edges>
      </equation*>
    </unfolded-io>
  </session>

  The command <with|font-family|tt|delete_vertex> takes two arguments, a
  graph <math|G<around|(|V,E|)>> and a single label <math|v> or a list of
  labels <math|L>, and returns the graph

  <\equation*>
    G<rprime|'>*<around|(|V\<setminus\><around|{|v|}>,<around|{|e\<in\>E:e*<text|is
    not incident to >v|}>|)>
  </equation*>

  or, if <math|L> is given,

  <\equation*>
    G<rprime|''>*<around|(|V\<setminus\>L,<around|{|e\<in\>E:e*<text|is not
    incident to any >v\<in\>L|}>|)>.
  </equation*>

  If any of the specified vertices does not belong to <math|G>, an error is
  returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_vertex(K5,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_vertex(K5,[2,3])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Modifying edges of a graph>

  <subsection|Adding and removing edges>

  For adding and removing edges or arcs to/from graphs use the commands
  <kbd|add_edge><index|<kbd|add_edge>> or <kbd|add_arc><index|<kbd|add_arc>>
  and <kbd|delete_edge><index|<kbd|delete_edge>> or
  <kbd|delete_arc><index|<kbd|delete_arc>>, respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|add_edge(G,e\|E\|T)>>>|<row|<cell|>|<cell|<verbatim|add_arc(G,e\|E\|T)>>>|<row|<cell|>|<cell|<verbatim|delete_edge(G,e\|E\|T)>>>|<row|<cell|>|<cell|<verbatim|delete_arc(G,e\|E\|T)>>>>>>

  The command <with|font-family|tt|add_edge> takes two arguments, an
  undirected graph <math|G> and an edge <math|e> or a list of edges <math|E>
  or a trail of edges <math|T> (entered as a list of vertices), and returns
  the copy of <math|G> with the specified edges inserted. Edge insertion
  implies that its endpoints will be created if they are not already present
  in <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C4:=cycle_graph(4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_edge(C4,[1,3])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_edge(C4,[1,3,5,7])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>
  </session>

  The command <with|font-family|tt|add_arc> works similarly to
  <with|font-family|tt|add_edge> but applies only to directed graphs. Note
  that the order of endpoints in an arc matters.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_arc(digraph(trail(a,b,c,d,a)),[[a,c],[b,d]])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 6 arcs>
      </equation*>
    </unfolded-io>
  </session>

  When adding edge/arc to a weighted graph, its weight should be specified
  alongside its endpoints, or it will be assumed that it equals to 1.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_edge(graph(%{[[1,2],5],[[3,4],6]%}),[[2,3],7])
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  The commands <verbatim|delete_edge> and <verbatim|delete_arc> take two
  arguments, the input graph <math|G> and an edge/arc <math|e> or a list of
  edges/arcs <math|E> or a trail of edges <math|T>. It returns a copy of
  <math|G> in which the specified edges/arcs are removed. Note that this
  operation does not change the set of vertices of <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K33:=relabel_vertices(complete_graph(3,3),[A,B,C,D,E,F])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(K33,[A,D])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_edge(K33,[A,D])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 8 edges>
      </equation*>
    </unfolded-io>
  </session>

  Note that <math|G> itself is not changed.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(K33,[B,D])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_edge(K33,[[A,D],[B,D]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      DG:=digraph(trail(1,2,3,4,5,2,4))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_arc(DG,[[2,3],[4,5],[5,2]])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      delete_arc(DG,[3,4,5,2])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 3 arcs>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Accessing and modifying edge weights>

  The commands <kbd|get_edge_weight><index|<kbd|get_edge_weight>> and
  <with|font-family|tt|set_edge_weight><index|<with|font-family|tt|set_edge_weight>>
  are used to access and modify the weight of an edge in a weighted graph,
  respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|set_edge_weight(G,e,w)>>>|<row|<cell|>|<cell|<verbatim|set_edge_weight(G,e)>>>>>>

  <with|font-family|tt|set_edge_weight> takes three arguments: a weighted
  graph <math|G<around|(|V,E|)>>, edge <math|e\<in\>E> and the new weight
  <math|w>, which may be any number. It returns the modified copy of
  <math|G>.

  The command <with|font-family|tt|get_edge_weight> takes two arguments, a
  weighted graph <math|G<around|(|V,E|)>> and an edge or arc <math|e\<in\>E>.
  It returns the weight of <math|e>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=set_edge_weight(graph(%{[[1,2],4],[[2,3],5]%}),[1,2],6)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      get_edge_weight(G,[1,2])
    <|unfolded-io>
      <\equation*>
        6
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Contracting edges>

  The command <with|font-family|tt|contract_edge><index|<with|font-family|tt|contract_edge>>
  is used for <hlink|<rigid|contracting edges>|https://en.wikipedia.org/wiki/Edge_contraction>
  in undirected graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|contract_edge(G,e)>>>>>>

  <with|font-family|tt|contract_edge> takes two arguments, an undirected
  graph <math|G<around|(|V,E|)>> and an edge
  <math|e=<around|(|v,w|)>\<in\>E>, and merges <math|v> and <math|w> to a
  single vertex, deleting the edge <math|e>. The resulting vertex inherits
  the label of <math|v>. The modified copy of <math|G> is returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      contract_edge(K5,[1,2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>
  </session>

  To contract a set <math|<around|{|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|k>|}>\<subset\>E>
  of edges in <math|G>, none two of which are incident (i.e.<nbsp>when the
  given set is a matching in <math|G>), one can use the
  <with|font-family|tt|foldl> command. In the following example, the complete
  graph <math|K<rsub|5>> is obtained from Petersen graph by edge contraction.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=foldl(contract_edge,P,[0,5],[1,6],[2,7],[3,8],[4,9])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/complete.eps|35%|||>>

  <subsection|Subdividing edges>

  The command <with|font-family|tt|subdivide_edges><index|<with|font-family|tt|subdivide_edges>>
  is used for <hlink|<rigid|graph subdivision>|https://en.wikipedia.org/wiki/Homeomorphism_(graph_theory)#Subdivision_and_smoothing>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|subdivide_edges(G,e\|S)>>>|<row|<cell|>|<cell|<verbatim|subdivide_edges(G,e\|S,r)>>>>>>

  <with|font-family|tt|subdivide_edges> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>>, a single edge/arc <math|e\<in\>E> or a list of
  edges/arcs <math|S\<subset\>E> and optionally a positive integer <math|r>
  (which defaults to 1). Each of the specified edges/arcs will be subdivided
  with exactly <math|r> new vertices, labeled with the smallest available
  nonnegative integers. The resulting graph, which is homeomorphic to
  <math|G>, is returned.

  If the endpoints of the edge being subdivided have valid coordinates, the
  coordinates of the inserted vertices will be computed accordingly.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=subdivide_edges(G,[2,3])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 11 vertices and 16 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=subdivide_edges(G,[[1,2],[3,4]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 13 vertices and 18 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=subdivide_edges(G,[0,1],2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/subdiv.eps|35%|||>>

  <section|Using attributes>

  <subsection|Graph attributes>

  The graph structure maintains a set of attributes as tag-value pairs which
  can be accessed and/or modified by using the commands
  <verbatim|set_graph_attribute><index|<with|font-family|tt|set_graph_attribute>>,
  <verbatim|get_graph_attribute><index|<with|font-family|tt|get_graph_attribute>>,
  <verbatim|list_graph_attributes><index|<with|font-family|tt|list_graph_attributes>>
  and <verbatim|discard_graph_attribute><index|<with|font-family|tt|discard_graph_attribute>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<cwith|1|1|1|1|cell-tborder|0ln>|<cwith|8|8|1|1|cell-bborder|0ln>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|1|1|cell-rborder|0ln>|<cwith|1|-1|2|2|cell-lborder|0ln>|<table|<row|<cell|Syntax:>|<cell|<verbatim|set_graph_attribute(G,tag1=value1,tag2=value2,...)>>>|<row|<cell|>|<cell|<verbatim|set_graph_attribute(G,[tag1=value1,tag2=value2,...])>>>|<row|<cell|>|<cell|<verbatim|set_graph_attribute(G,[tag1,tag2,...],[value1,value2,...])>>>|<row|<cell|>|<cell|<verbatim|get_graph_attribute(G,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|get_graph_attribute(G,[tag1,tag2,...])>>>|<row|<cell|>|<cell|<verbatim|list_graph_attributes(G)>>>|<row|<cell|>|<cell|<verbatim|discard_graph_attribute(G,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|discard_graph_attribute(G,[tag1,tag2,...])>>>>>>

  The command <with|font-family|tt|set_graph_attribute> is used for modifying
  the existing graph attributes or adding new ones. It takes two arguments, a
  graph <math|G> and a sequence or list of graph attributes in form
  <with|font-family|tt|tag=value> where <with|font-family|tt|tag> is a
  string. Alternatively, attributes may be specified as a sequence of two
  lists <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attribute slots, which are meant to represent some
  global properties of the graph <math|G>, and returns the modified copy of
  <math|G>.

  The previously set graph attribute values can be fetched with the command
  <with|font-family|tt|get_graph_attribute> which takes two arguments: a
  graph <math|G> and a sequence or list of tags. The corresponding values
  will be returned in a sequence or list, respectively. If an attribute is
  not set, <with|font-family|tt|undef> is returned as its value.

  To list all graph attributes of <math|G> for which the values are set, use
  the command <with|font-family|tt|list_graph_attributes> which takes
  <math|G> as its only argument.

  To discard a graph attribute, use the command
  <with|font-family|tt|discard_graph_attribute>. It takes two arguments: a
  graph <math|G> and a sequence or list of tags to be cleared, and returns
  the modified copy of <math|G>.

  Two tags being used by the <abbr|CAS> commands are <samp|directed> and
  <samp|weighted>, so it is not advisable to overwrite their values using
  this command; use the <verbatim|<hlink|make_directed|#make-directed>>,
  <verbatim|<hlink|make_weighted|#make-weighted>> and
  <verbatim|<hlink|underlying_graph|#underlying-graph>> commands instead.
  Another attribute used internally is <samp|name>, which holds the name of
  the respective graph (as a string).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(trail(1,2,3,1))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=set_graph_attribute(G,"name"="C3","message"="this is some text")
    <|unfolded-io>
      <\equation*>
        <text|C3: a directed unweighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      get_graph_attribute(G,"message")
    <|unfolded-io>
      <\equation*>
        <with|math-font-family|rm|this is some text>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_graph_attributes(G)
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|directed>=<text|true>,<math-up|weighted>=<text|false>,<math-up|name>=<with|math-font-family|rm|C3>,<math-up|message>=<with|math-font-family|rm|this
        is some text>|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=discard_graph_attribute(G,"message")
    <|unfolded-io>
      <\equation*>
        <text|C3: a directed unweighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_graph_attributes(G)
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|directed>=<text|true>,<math-up|weighted>=<text|false>,<math-up|name>=<with|math-font-family|rm|C3>|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Vertex attributes><label|vertex-attribute>

  For every vertex of a graph, the list of attributes in form of tag-value
  pairs is maintained, which can be accessed/modified by using the commands
  <verbatim|set_vertex_attribute><index|<with|font-family|tt|set_vertex_attribute>>,
  <verbatim|get_vertex_attribute><index|<with|font-family|tt|get_vertex_attribute>>,
  <verbatim|list_vertex_attributes><index|<with|font-family|tt|list_vertex_attributes>>
  and <verbatim|discard_vertex_attribute><index|<with|font-family|tt|discard_vertex_attribute>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|set_vertex_attribute(G,v,tag1=value1,tag2=value2,...)>>>|<row|<cell|>|<cell|<verbatim|set_vertex_attribute(G,v,[tag1=value1,tag2=value2,...])>>>|<row|<cell|>|<cell|<verbatim|set_vertex_attribute(G,v,[tag1,tag2,...],[value1,value2,...])>>>|<row|<cell|>|<cell|<verbatim|get_vertex_attribute(G,v,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|get_vertex_attribute(G,v,[tag1,tag2,...])>>>|<row|<cell|>|<cell|<verbatim|list_vertex_attributes(G,v)>>>|<row|<cell|>|<cell|<verbatim|discard_vertex_attribute(G,v,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|discard_vertex_attribute(G,v,[tag1,tag2,...])>>>>>>

  The command <with|font-family|tt|set_vertex_attribute> is used for
  modifying the existing vertex attributes or adding new ones. It takes three
  arguments, a graph <math|G<around|(|V,E|)>>, a vertex <math|v\<in\>V> and a
  sequence or list of attributes in form <with|font-family|tt|tag=value>
  where <with|font-family|tt|tag> is a string. Alternatively, attributes may
  be specified as a sequence of two lists
  <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attributes of the vertex <math|v> and returns the
  modified copy of <math|G>.

  The previously set attribute values for <math|v> can be fetched with the
  command <with|font-family|tt|get_vertex_attribute> which takes three
  arguments: <math|G>, <math|v> and a sequence or list of tags. The
  corresponding values will be returned in a sequence or list, respectively.
  If an attribute is not set, <with|font-family|tt|undef> is returned as its
  value.

  To list all attributes of <math|v> for which the values are set, use the
  command <with|font-family|tt|list_vertex_attributes> which takes two
  arguments, <math|G> and <math|v>.

  The command <with|font-family|tt|discard_vertex_attribute> is used for
  discarding attribute(s) assigned to some vertex <math|v\<in\>V>. It takes
  three arguments: <math|G>, <math|v> and a sequence or list of tags to be
  cleared, and returns the modified copy of <math|G>.

  The attributes <samp|label>, <samp|color>, <samp|shape> and <samp|pos> are
  also used internally. These hold the vertex label, color, shape and
  coordinates in a drawing, respectively. If the color is not set for a
  vertex, the latter is drawn in yellow. The <samp|shape> attribute may have
  one of the following values: <samp|square>, <samp|triangle>,
  <samp|diamond>, <samp|star> or <samp|plus>. If the <samp|shape> attribute
  is not set or has a different value, the circled shape is applied when
  drawing the vertex.

  The following example shows how to change individual labels and colors.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=complete_binary_tree(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=set_vertex_attribute(T,5,"label"="root","color"=red)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T,tree="root")
    </input>
  </session>

  <center|<image|images/rooted.eps|35%|||>>

  A vertex may also hold custom attributes.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=set_vertex_attribute(T,"root","depth"=3,"shape"="square")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_vertex_attributes(T,"root")
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|label>=<math-up|root>,<math-up|color>=r*e*d,<math-up|shape>=<math-up|square>,<math-up|depth>=3|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=discard_vertex_attribute(T,"root","color")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_vertex_attributes(T,"root")
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|label>=<math-up|root>,<math-up|shape>=<math-up|square>,<math-up|depth>=3|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Edge attributes>

  For every edge of a graph, the list of attributes in form of key-value
  pairs is maintained, which can be accessed and/or modified by using the
  commands <verbatim|set_edge_attribute><index|<with|font-family|tt|set_edge_attribute>>,
  <verbatim|get_edge_attribute><index|<with|font-family|tt|get_edge_attribute>>,
  <verbatim|list_edge_attributes><index|<with|font-family|tt|list_edge_attributes>>
  and <verbatim|discard_edge_attribute><index|<with|font-family|tt|discard_edge_attribute>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|set_edge_attribute(G,e,tag1=value1,tag2=value2,...)>>>|<row|<cell|>|<cell|<verbatim|set_edge_attribute(G,e,[tag1=value1,tag2=value2,...])>>>|<row|<cell|>|<cell|<verbatim|set_edge_attribute(G,e,[tag1,tag2,...],[value1,value2,...])>>>|<row|<cell|>|<cell|<verbatim|get_edge_attribute(G,e,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|get_edge_attribute(G,e,[tag1,tag2,...])>>>|<row|<cell|>|<cell|<verbatim|list_edge_attributes(G,e)>>>|<row|<cell|>|<cell|<verbatim|discard_edge_attribute(G,e,tag1,tag2,...)>>>|<row|<cell|>|<cell|<verbatim|discard_edge_attribute(G,e,[tag1,tag2,...])>>>>>>

  The command <with|font-family|tt|set_edge_attribute> is used for modifying
  the existing edge attributes or adding new ones. It takes three arguments,
  a graph <math|G<around|(|V,E|)>>, an edge/arc <math|e\<in\>E> and a
  sequence or list of attributes in form <with|font-family|tt|tag=value>
  where <with|font-family|tt|tag> is a string. Alternatively, attributes may
  be specified as a sequence of two lists
  <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attributes of the edge/arc <math|e> and returns the
  modified copy of <math|G>.

  The previously set attribute values for <math|e> can be fetched with the
  command <with|font-family|tt|get_edge_attribute> which takes three
  arguments: <math|G>, <math|e> and a sequence or list of tags. The
  corresponding values will be returned in a sequence or list, respectively.
  If some attribute is not set, <with|font-family|tt|undef> is returned as
  its value.

  To list all attributes of <math|e> for which the values are set, use the
  command <with|font-family|tt|list_edge_attributes> which takes two
  arguments, <math|G> and <math|e>.

  To discard attribute(s) assigned to <math|e> call the command
  <with|font-family|tt|discard_edge_attribute>, which takes three arguments:
  <math|G>, <math|e> and a sequence or list of tags to be cleared, and
  returns the modified copy of <math|G>.

  The attributes <samp|weight>, <samp|color>, <samp|style>, <samp|pos> and
  <samp|temp> are also used internally. They hold the edge weight, color,
  line style, the coordinates of the weight label anchor (and also the
  coordinates of the arrow) and <verbatim|true> if the edge is temporary. If
  the color attribute is not set for an edge, the latter is drawn in blue,
  unless it is a temporary edge, in which case it is drawn in light gray. The
  <samp|style> attribute may have one of the following values: <samp|dashed>,
  <samp|dotted> or <samp|bold>. If the <samp|style> attribute is not set or
  has a different value, the solid line style is applied when drawing the
  edge.

  The following example illustrates the possibilities of using edge
  attributes.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=complete_binary_tree(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=set_edge_attribute(T,[1,4],"cost"=12.8,"message"="this is some
      text")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_edge_attributes(T,[1,4])
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|cost>=12.8,<math-up|message>=<with|math-font-family|rm|this
        is some text>|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=discard_edge_attribute(T,[1,4],"message")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=set_edge_attribute(T,[1,4],"style"="dotted","color"=magenta)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      list_edge_attributes(T,[1,4])
    <|unfolded-io>
      <\equation*>
        <around|[|<math-up|color>=m*a*g*e*n*t*a,<math-up|style>=<math-up|dotted>,<math-up|cost>=12.8|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=set_edge_attribute(T,[5,11],"temp"=true)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T)
    </input>
  </session>

  <center|<image|images/dashed.eps|35%|||>>

  <chapter|Import and export>

  <section|Importing graphs>

  <subsection|Loading graphs from <with|font-family|ss|dot>
  files><label|import-graph>

  The command <verbatim|import_graph><index|<verbatim|import_graph>> is used
  for importing a graph from text file in
  <hlink|<samp|dot>|https://en.wikipedia.org/wiki/DOT_(graph_description_language)>
  format.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|import_graph(filename)>>>>>>

  <with|font-family|tt|import_graph> takes a string
  <with|font-family|tt|filename> as its only argument and returns the graph
  constructed from instructions written in the file
  <with|font-family|tt|filename> or <with|font-family|tt|undef> on failure.
  The passed string should contain the path to a file in
  <with|font-family|ss|dot> format. The file extension
  <with|font-family|tt|.dot> may be omitted in the
  <with|font-family|tt|filename> since <with|font-family|ss|dot> is the only
  supported format. The alternative extension is <verbatim|.gv><\footnote>
    Although it is recommended to use <verbatim|.gv> as the extension for
    <samp|dot> files to avoid a certain confusion between different file
    types, <samp|Giac> uses the <verbatim|.dot> extension because it
    coincides with the format name. This may change in the future.
  </footnote>, which must be explicitly specified.

  If a relative path to the file is specified, i.e.<nbsp>if it does not
  contain a leading forward slash, the current working directory (which can
  be obtained by calling the <with|font-family|tt|pwd> command) will be used
  as the reference. The working directory can be changed by using the command
  <with|font-family|tt|cd>.

  For example, assume that the file <with|font-family|tt|example.dot> is
  saved in the directory <with|font-family|tt|path/to/dot/> with the
  following contents:

  <\verbatim-code>
    graph "Example graph" {

    \ \ a [label="Foo"];

    \ \ b [shape=diamond,color=red];

    \ \ a -- b [style=bold];

    \ \ b -- c [color=green];

    \ \ b -- d [style=dotted];

    }
  </verbatim-code>

  To import the graph, input:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=import_graph("path/to/dot/example.dot")
    <|unfolded-io>
      <\equation*>
        <text|Example graph: an undirected unweighted graph with 4 vertices
        and 3 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/ex.eps|35%|||>>

  <subsection|The <with|font-family|ss|dot> file format overview>

  <samp|Giac> has some basic support for <hlink|<rigid|<with|font-family|ss|dot>
  language>|https://www.graphviz.org/doc/info/lang.html>. Each <samp|dot>
  file is used to hold exactly one graph and should consist of a single
  instance of the following environment:

  <\verbatim-code>
    strict? (graph \| digraph) name? {

    \ \ \ \ ...

    }
  </verbatim-code>

  The keyword <with|font-family|tt|strict> may be omitted, as well as the
  <with|font-family|tt|name> of the graph, as indicated by the question
  marks. The former is used to differentiate between simple graphs (strict)
  and multigraphs (non-strict). Since <samp|Giac> supports only simple
  graphs, <with|font-family|tt|strict> is redundant.

  For specifying undirected graphs the keyword <with|font-family|tt|graph> is
  used, while the <with|font-family|tt|digraph> keyword is used for
  undirected graphs.

  The <with|font-family|tt|graph>/<with|font-family|tt|digraph> environment
  contains a series of instructions describing how the graph should be built.
  Each instruction ends with the semicolon (<with|font-family|tt|;>) and has
  one of the following forms.

  <center|<tabular|<tformat|<cwith|1|1|1|-1|cell-tborder|0ln>|<cwith|1|1|1|-1|cell-bborder|1ln>|<cwith|2|2|1|-1|cell-tborder|1ln>|<cwith|1|1|1|1|cell-lborder|0ln>|<cwith|1|1|2|2|cell-rborder|0ln>|<table|<row|<cell|<em|syntax>>|<cell|<em|creates>>>|<row|<cell|<with|font-family|tt|vertex_name<nbsp>[attributes]?>>|<cell|isolated
  vertices>>|<row|<cell|<with|font-family|tt|V1<nbsp>\<less\>edgeop\<gtr\><nbsp>V2<nbsp>\<less\>edgeop\<gtr\>><nbsp>\<ldots\>
  <nbsp><kbd|\<less\>edgeop\<gtr\><nbsp>Vk<nbsp>[attributes]?>>|<cell|edges
  and trails>>|<row|<cell|<with|font-family|tt|graph<nbsp>[attributes]>>|<cell|graph
  attributes>>>>>>

  Here, <with|font-family|tt|attributes> is a comma-separated list of
  tag-value pairs in form <with|font-family|tt|tag=value>,
  <with|font-family|tt|\<less\>edgeop\<gtr\>> is <rigid|<verbatim|-->> for
  undirected and <rigid|<with|font-family|tt|-\<gtr\>>> for directed graphs.
  Each of <with|font-family|tt|V1>, <with|font-family|tt|V2> etc.<nbsp>is
  either a vertex name or a set of vertex names in form
  <with|font-family|tt|{vertex_name1 vertex_name2<nbsp>...}>. In the case a
  set is specified, each vertex from that set is connected to the neighbor
  operands. Every specified vertex will be created if it does not exist yet.

  Lines beginning with <with|font-family|tt|#> are ignored. C-like comments
  are recognized and skipped as well.

  Using the <with|font-family|ss|dot> syntax it is easy to specify a graph
  with adjacency lists. For example, the following is the contents of a file
  which defines the octahedral graph with 6 vertices and 12 edges.

  <\verbatim-code>
    # octahedral graph

    graph "octahedron" {

    \ \ 1 -- {3 6 5 4};

    \ \ 2 -- {3 4 5 6};

    \ \ 3 -- {5 6};

    \ \ 4 -- {5 6};

    }
  </verbatim-code>

  <section|Exporting graphs>

  The command <with|font-family|tt|export_graph> is used for saving graphs to
  disk in <with|font-family|ss|dot> or <LaTeX> format.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|export_graph(G,filename)>>>|<row|<cell|>|<cell|<verbatim|export_graph(G,filename,latex[=\<less\>params\<gtr\>])>>>>>>

  The argument <verbatim|filename> should be a string containing a path to
  the desired destination file (which is created if it does not exist). The
  remark on relative paths in Section<nbsp><reference|import-graph> applies
  here as well.

  <subsection|Saving graphs in <with|font-family|ss|dot> format>

  <with|font-family|tt|export_graph<index|<with|font-family|tt|export_graph>>>
  takes two mandatory arguments, a graph <math|G> and a string
  <with|font-family|tt|filename>, and writes <math|G> to the file specified
  by <with|font-family|tt|filename>. If only two arguments are given the
  graph is saved in <with|font-family|ss|dot> format. The file name may be
  entered with or without <with|font-family|tt|.dot> extension. The command
  returns 1 on success and 0 on failure.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      export_graph(G,"path/to/dot/copy_of_example")
    <|unfolded-io>
      <\equation*>
        1
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Saving graph drawings in <LaTeX> format>

  When calling <kbd|export_graph>, an optional third argument in form
  <with|font-family|tt|latex[=\<less\>params\<gtr\>]> may be given. In that
  case the drawing of <math|G> (obtained by calling the
  <with|font-family|tt|draw_graph> command) will be saved to the <LaTeX> file
  indicated by <with|font-family|tt|filename> (the extension
  <with|font-family|tt|.tex> may be omitted). Optionally, one can specify a
  parameter or list of parameters <with|font-family|tt|params> which will be
  passed to <with|font-family|tt|draw_graph>.

  For example, let us create a picture of the Sierpi«ski sieve graph of order
  <math|n=5>, i.e.<nbsp>the graph <math|S T<rsub|3><rsup|5>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=sierpinski_graph(5,3,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 123 vertices and 243 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      export_graph(G,"some/directory/st53.tex",latex=[spring,labels=false])
    <|unfolded-io>
      <\equation*>
        1
      </equation*>
    </unfolded-io>
  </session>

  The <LaTeX> file obtained by exporting a graph is easily converted into an
  <abbr|EPS> file, which can subsequently be inserted<\footnote>
    Alternatively, a PSTricks picture from the body of the <verbatim|.tex>
    file can be copied to some other <LaTeX> document.
  </footnote> in a paper, report or some other document. A Linux user simply
  needs to launch a terminal emulator, navigate to the directory in which the
  exported file, in this case <with|font-family|tt|st53.tex>, is stored and
  enter the following command:

  <\shell-code>
    latex st53.tex && dvips st53.dvi && ps2eps st53.ps
  </shell-code>

  This will produce the (properly cropped) <with|font-family|tt|st53.eps>
  file in the same directory. Afterwards, it is recommended to enter

  <\shell-code>
    <\shell>
      rm st53.tex st53.aux st53.log st53.dvi st53.ps
    </shell>
  </shell-code>

  to delete the intermediate files. The above two commands can be combined in
  a simple shell script which takes the name of the exported file (without
  the extension) as its input argument:

  <\shell-code>
    #!/bin/bash

    # convert LaTeX to EPS

    latex $1.tex

    dvips $1.dvi

    ps2eps $1.ps

    rm $1.tex $1.aux $1.log $1.dvi $1.ps
  </shell-code>

  Assuming that the script is stored under the name
  <with|font-family|tt|latex2eps> in the same directory as
  <with|font-family|tt|st53.tex>, to do the conversion it is enough to input:

  <\shell-code>
    bash latex2eps st53
  </shell-code>

  The drawing produced in our example is shown in
  Figure<nbsp><reference|st53>.<float|float|thb|<big-figure|<image|images/st53.eps|160pt|138pt||>|<label|st53>drawing
  of the Sierpi«ski graph <math|S T<rsub|3><rsup|5>> using <LaTeX> and
  PSTricks>>

  <chapter|Graph properties>

  <section|Basic properties>

  <subsection|Determining the type of a graph>

  The commands <verbatim|is_directed><index|<verbatim|is_directed>> and
  <verbatim|is_weighted><index|<verbatim|is_weighted>> are used for
  determining the type of a graph: whether is it directed or not
  resp.<nbsp>weighted or not.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_directed(G)>>>|<row|<cell|>|<cell|<verbatim|is_weighted(G)>>>>>>

  Both commands take a graph <math|G> as their only argument.
  <verbatim|is_directed> resp.<nbsp><verbatim|is_weighted> returns
  <verbatim|true> if <math|G> is directed resp.<nbsp>weighted, else it
  returns <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5,1,3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_directed(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_directed(make_directed(G))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_weighted(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_weighted(make_weighted(G,randmatrix(5,5,99)))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Listing vertices and edges><label|vertices-edges>

  The command <verbatim|vertices><index|<verbatim|vertices>> or
  <verbatim|graph_vertices><index|<verbatim|graph_vertices>>
  resp.<nbsp><verbatim|edges><index|<verbatim|edges>> is used for extracting
  the set of vertices resp.<nbsp>the set of edges from a graph. To obtain the
  number of vertices resp.<nbsp>the number of edges, use the
  <verbatim|number_of_vertices><index|<verbatim|number_of_vertices>>
  resp.<nbsp>the <verbatim|number_of_edges><index|<verbatim|number_of_edges>>
  command.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|vertices(G)>>>|<row|<cell|>|<cell|<verbatim|graph_vertices(G)>>>|<row|<cell|>|<cell|<verbatim|edges(G)>>>|<row|<cell|>|<cell|<verbatim|edges(G,weights)>>>|<row|<cell|>|<cell|<verbatim|number_of_vertices(G)>>>|<row|<cell|>|<cell|<verbatim|number_of_edges(G)>>>>>>

  <verbatim|vertices> or <verbatim|graph_vertices> takes a graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the set of
  vertices <math|V> in the same order in which they were created.

  <verbatim|edges> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally the identifier <verbatim|weights>.
  The command returns the set of edges <math|E> (in a non-meaningful order).
  If <verbatim|weights> is specified, each edge is paired with the
  corresponding weight (in this case <math|G> must be a weighted graph).

  <verbatim|number_of_vertices> resp.<nbsp><verbatim|number_of_edges> takes
  the input graph <math|G<around*|(|V,E|)>> as its only argument and returns
  <math|<around*|\||V|\|>> resp.<nbsp><math|<around*|\||E|\|>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=hypercube_graph(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(G)
    <|unfolded-io>
      <\equation*>
        <around|[|<with|math-font-family|rm|00>,<with|math-font-family|rm|01>,<with|math-font-family|rm|10>,<with|math-font-family|rm|11>|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=graph("coxeter")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 28 vertices and 42 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(C)
    <|unfolded-io>
      <\equation*>
        <around|[|<with|math-font-family|rm|a1>,<with|math-font-family|rm|a2>,<with|math-font-family|rm|a7>,<with|math-font-family|rm|z1>,<with|math-font-family|rm|a3>,<with|math-font-family|rm|z2>,<with|math-font-family|rm|a4>,<with|math-font-family|rm|z3>,<with|math-font-family|rm|a5>,<with|math-font-family|rm|z4>,<with|math-font-family|rm|a6>,<with|math-font-family|rm|z5>,<with|math-font-family|rm|z6>,<with|math-font-family|rm|z7>,<with|math-font-family|rm|b1>,<with|math-font-family|rm|b3>,<with|math-font-family|rm|b6>,<with|math-font-family|rm|b2>,<with|math-font-family|rm|b4>,<with|math-font-family|rm|b7>,<with|math-font-family|rm|b5>,<with|math-font-family|rm|c1>,<with|math-font-family|rm|c4>,<with|math-font-family|rm|c5>,<with|math-font-family|rm|c2>,<with|math-font-family|rm|c6>,<with|math-font-family|rm|c3>,<with|math-font-family|rm|c7>|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_vertices(C), number_of_edges(C)
    <|unfolded-io>
      <\equation*>
        28,42
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=digraph([[0,2.32,0,0.25],[0,0,0,1.32],[0,0.50,0,0],[0.75,0,3.34,0]])
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 4 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(H)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|3>>|<row|<cell|1>|<cell|3>>|<row|<cell|2>|<cell|1>>|<row|<cell|3>|<cell|0>>|<row|<cell|3>|<cell|2>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(H,weights)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|<around|[|0,1|]>,2.32|]>,<around|[|<around|[|0,3|]>,0.25|]>,<around|[|<around|[|1,3|]>,1.32|]>,<around|[|<around|[|2,1|]>,0.5|]>,<around|[|<around|[|3,0|]>,0.75|]>,<around|[|<around|[|3,2|]>,3.34|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Equality of graphs><label|graph-equal>

  Two graphs are considered <strong|equal> if they are both (un)weighted and
  (un)directed and if the commands <verbatim|<hlink|vertices|#vertices-edges>>
  and <verbatim|<hlink|edges|#vertices-edges>> give the same results for both
  graphs. To determine whether two graphs are equal use the command
  <verbatim|graph_equal><index|<verbatim|graph_equal>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_equal(G1,G2)>>>>>>

  <verbatim|graph_equal> takes two arguments, graphs <math|G<rsub|1>> and
  <math|G<rsub|2>>, and returns <verbatim|true> if <math|G<rsub|1>> is equal
  to <math|G<rsub|2>> with respect to the above definition. Else, it returns
  <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph([1,2,3],%{[1,2],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=graph([1,3,2],%{[1,2],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_equal(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G3:=graph(trail(1,2,3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_equal(G1,G3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G4:=digraph(trail(1,2,3))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 3 vertices and 2 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_equal(G1,G4)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Vertex degrees>

  The command <verbatim|vertex_degree><index|<verbatim|vertex_degree>> is
  used for computing the degree of a vertex, i.e.<nbsp>counting the vertices
  adjacent to it. The related specialized commands are
  <verbatim|vertex_out_degree><index|<verbatim|vertex_out_degree>>,
  <verbatim|vertex_in_degree><index|<verbatim|vertex_in_degree>>,
  <verbatim|degree_sequence><index|<verbatim|degree_sequence>>,
  <verbatim|minimum_degree><index|<verbatim|minimum_degree>> and
  <verbatim|maximum_degree><index|<verbatim|maximum_degree>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|vertex_degree(G,v)>>>|<row|<cell|>|<cell|<verbatim|vertex_in_degree(G,v)>>>|<row|<cell|>|<cell|<verbatim|vertex_out_degree(G,v)>>>|<row|<cell|>|<cell|<verbatim|degree_sequence(G)>>>|<row|<cell|>|<cell|<verbatim|minimum_degree(G,v)>>>|<row|<cell|>|<cell|<verbatim|maximum_degree(G,v)>>>>>>

  <verbatim|vertex_degree> takes two arguments, a graph
  <math|G<around*|(|V,E|)>> and a vertex <math|v\<in\>V>, and returns the
  cardinality of the set <math|<around*|{|w\<in\>V\<of\><around*|(|v,w|)>\<in\>E|}>>,
  i.e.<nbsp>the number of vertices in <math|V> which are adjacent to
  <math|v>. Note that the edge directions are ignored in case <math|G> is a
  digraph.

  When dealing with directed graphs, one can also use the specialized command
  <verbatim|vertex_out_degree> resp.<nbsp><verbatim|vertex_in_degree> which
  takes the same arguments as <verbatim|vertex_degree> but returns the number
  of arcs <math|<around*|(|v,w|)>\<in\>E> resp.<nbsp>the number of arcs
  <math|<around*|(|w,v|)>\<in\>E>, where <math|w\<in\>V>.

  To obtain the list of degrees of all vertices <math|v\<in\>V>, use the
  command <verbatim|degree_sequence> which takes a graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the list of
  degrees of vertices from <math|V> in the same order as returned by the
  command <verbatim|vertices>. If <math|G> is a digraph, arc directions are
  ignored.

  To compute the minimum vertex degree <math|\<delta\><around*|(|G|)>> and
  the maximum vertex degree <math|\<Delta\><around*|(|G|)>> in an undirected
  graph <math|G>, use the commands <verbatim|minimum_degree> and
  <verbatim|maximum_degree>, respectively. Both commands take <math|G> as the
  only argument and return <math|\<delta\><around*|(|G|)>>
  resp.<nbsp><math|\<Delta\><around*|(|G|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,1,5,6,7,1,8,9,4))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 9 vertices and 11 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/graph1.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_degree(G,1)
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      degree_sequence(G)
    <|unfolded-io>
      <\equation*>
        <around|[|5,2,2,3,2,2,2,2,2|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=random_tournament([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T)
    </input>
  </session>

  <center|<image|images/tournament.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_out_degree(T,1)
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_in_degree(T,5)
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>
  </session>

  The command line below shows that Petersen graph is cubic (3-regular).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      minimum_degree(P), maximum_degree(P)
    <|unfolded-io>
      <\equation*>
        3,3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(P,3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Regular graphs><label|is-regular>

  The command <verbatim|is_regular><index|<verbatim|is_regular>> is used for
  determining whether a graph is <hlink|regular|https://en.wikipedia.org/wiki/Regular_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_regular(G)>>>|<row|<cell|>|<cell|<verbatim|is_regular(G,d)>>>>>>

  <verbatim|is_regular> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally a nonnegative integer or an
  unassigned identifier <math|d>. If <math|G> is undirected, the return value
  is <verbatim|true> if <math|\<delta\><rsub|G>=\<Delta\><rsub|G>>,
  i.e.<nbsp>if the minimal vertex degree is equal to the maximal vertex
  degree in <math|G>, otherwise <verbatim|false> is returned. If <math|G> is
  a digraph, it is also required for each vertex <math|v\<in\>V> to have the
  same in- and out-degree. If the second argument is given, <math|G> is
  tested for <math|d>-regularity in case <math|d> is an integer, otherwise
  <math|\<Delta\><rsub|G>> is written to <math|d> in case the latter is an
  identifier and <math|G> is regular.

  The complexity of the algorithm is <math|O<around*|(|<around*|\||V|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(G,d)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      d
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(G,2)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(graph("grotzsch"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(%{[1,5],[1,6],[2,3],[2,4],[3,1],[3,4],[4,1],[4,5],[5,2],[5,6],[6,2],[6,3]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 6 vertices and 12 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/dreg.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(G,4)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=add_arc(delete_arc(G,[5,6]),[6,5])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 6 vertices and 12 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(H,4)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(underlying_graph(H))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Strongly regular graphs>

  The command <verbatim|is_strongly_regular><index|<verbatim|is_strongly_regular>>
  is used for determining whether a graph is <hlink|<rigid|strongly
  regular>|https://en.wikipedia.org/wiki/Strongly_regular_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_strongly_regular(G)>>>|<row|<cell|>|<cell|<verbatim|is_strongly_regular(G,srg)>>>>>>

  <verbatim|is_strongly_regular> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally an unassigned identifier
  <verbatim|srg>. It returns <verbatim|true> if <math|G> is regular and there
  are integers <math|\<lambda\>> and <math|\<mu\>> such that every two
  adjacent vertices resp.<nbsp>non-adjacent vertices in <math|V> have exactly
  <math|\<lambda\>> resp.<nbsp><math|\<mu\>> common neighbors. Else, it
  returns <verbatim|false>. If the second argument is given, the list
  <math|<around*|[|k,\<lambda\>,\<mu\>|]>>, where <math|k> is the degree of
  <math|G>, is stored to <verbatim|srg>.

  The complexity of the algorithm is <math|O<around*|(|k*<around*|\||V|\|><rsup|2>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("clebsch")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 40 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_regular(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_strongly_regular(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=graph("shrikhande")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 48 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_strongly_regular(H,s)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      s
    <|unfolded-io>
      <\equation*>
        <around|[|6,2,2|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_strongly_regular(cycle_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_strongly_regular(cycle_graph(6))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  \;

  <subsection|Vertex adjacency>

  The command <verbatim|has_edge><index|<verbatim|has_edge>> is used for
  checking whether two vertices in an undirected graph are adjacent. For
  digraphs, there is an analogous command
  <verbatim|has_arc><index|<verbatim|has_arc>>.

  The command <verbatim|neighbors><index|<verbatim|neighbors>> is used for
  obtaining the list of vertices in a graph that are adjacent to the
  particular vertex or the complete adjacency structure of the graph, in
  sparse form.

  <label|departures-arrivals>The command <verbatim|departures><index|<verbatim|departures>>
  resp.<nbsp><verbatim|arrivals><index|<verbatim|arrivals>> is used for
  obtaining all neighbors of a vertex <math|v> in a digraph which are the
  heads resp.<nbsp>the tails of the corresponding arcs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|has_edge(G,[u,v])>>>|<row|<cell|>|<cell|<verbatim|has_arc(G,[u,v])>>>|<row|<cell|>|<cell|<verbatim|neighbors(G)>>>|<row|<cell|>|<cell|<verbatim|neighbors(G,v)>>>|<row|<cell|>|<cell|<verbatim|departures(G)>>>|<row|<cell|>|<cell|<verbatim|departures(G,v)>>>|<row|<cell|>|<cell|<verbatim|arrivals(G)>>>|<row|<cell|>|<cell|<verbatim|arrivals(G,v)>>>>>>

  <verbatim|has_edge> takes two arguments, an undirected graph
  <math|G<around*|(|V,E|)>> and a list <verbatim|[u,v]> where
  <math|u,v\<in\>V>. The command returns <verbatim|true> if <math|u v\<in\>E>
  and <verbatim|false> otherwise. The syntax for <verbatim|has_arc> is the
  same, except now <math|G> is required to be directed. Note, however, that
  the order of vertices <math|u> and <math|v> matters in digraphs. The
  complexity of both algorithms is <math|O<around*|(|log
  <around*|\||V|\|>|)>>.

  <verbatim|neighbors> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally a vertex <math|v\<in\>V>. The
  command returns the list of neighbors of <math|v> in <math|G> if <math|v>
  is given. \ Otherwise, it returns the list of lists of neighbors for all
  vertices in <math|V>, in order of <verbatim|vertices(G)>. Note that edge
  directions are ignored in case <math|G> is a digraph.

  <verbatim|departures> resp.<nbsp><verbatim|arrivals> takes one or two
  arguments, a digraph <math|G<around*|(|V,E|)>> and optionally a vertex
  <math|v\<in\>V>, and returns the list <math|L<rsub|v>> containing all
  vertices <math|w\<in\>V> for which <math|v w\<in\>E> resp.<nbsp><math|w
  v\<in\>E>. If <math|v> is omitted, the list of lists <math|L<rsub|v>> for
  every <math|v\<in\>V> is returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5,2))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(G,[1,2])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(G,[2,1])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(G,[1,3])
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D:=digraph(trail(1,2,3,4,5,2,1))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,[1,2])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,[2,1])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,%{1,2%})
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,[4,5])
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,[5,4])
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_arc(D,%{4,5%})
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      neighbors(G,3)
    <|unfolded-io>
      <\equation*>
        <around|[|2,4|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      neighbors(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|2|]>,<around|[|1,3,5|]>,<around|[|2,4|]>,<around|[|3,5|]>,<around|[|2,4|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(trail(1,2,3,4,2,5,1,6,7,8,4))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 8 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/digraph7.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      departures(G,2); arrivals(G,2); departures(G,1); arrivals(G,1)
    <|unfolded-io>
      <\equation*>
        <around|[|3,5|]>,<around|[|1,4|]>,<around|[|2,6|]>,<around|[|5|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Tournament graphs><label|is-tournament>

  The command <verbatim|is_tournament><index|<verbatim|is_tournament>> is
  used for determining whether a graph is a
  <hlink|tournament|https://en.wikipedia.org/wiki/Tournament_(graph_theory)>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_tournament(G)>>>>>>

  <verbatim|is_tournament> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns <verbatim|true> if <math|G> is directed and for
  each pair of vertices <math|u,v\<in\>V> it is either <math|u v\<in\>E> or
  <math|v u\<in\>E>, i.e.<nbsp>there is exactly one arc between <math|u> and
  <math|v>. Else, it returns <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T1:=digraph(%{[1,2],[2,3],[3,1]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tournament(T1)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T2:=digraph(%{[1,2],[2,3],[3,1],[1,3]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 3 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tournament(T2)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Bipartite graphs><label|is-bipartite>

  The command <verbatim|is_bipartite><index|<verbatim|is_bipartite>> is used
  for determining if a graph is <hlink|bipartite|https://en.wikipedia.org/wiki/Bipartite_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_bipartite(G)>>>|<row|<cell|>|<cell|<verbatim|is_bipartite(G,P)>>>>>>

  <verbatim|is_bipartite> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally an unassigned identifier
  <verbatim|P>. It returns <verbatim|true> if there is a partition of
  <math|V> into two sets <math|S> and <math|T> such that every edge from
  <math|E> connects a vertex in <math|S> to one in <math|T>. Else, it returns
  <verbatim|false>. If the second argument is given and <math|G> is
  bipartite, the partition of <math|V> is stored to <verbatim|P> as a list of
  two lists of vertices, the first one containing the vertices from <math|S>
  and the second one containing vertices from <math|T>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K32:=complete_graph(3,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_bipartite(K32,bp)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      bp
    <|unfolded-io>
      <\equation*>
        <around|[|<around|[|0,1,2|]>,<around|[|3,4|]>|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(K32,bipartite)
    </input>
  </session>

  <center|<image|images/bip.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      adjacency_matrix(K32)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|5|5|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|1>>|<row|<cell|1>|<cell|1>|<cell|1>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|1>|<cell|1>|<cell|0>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=cycle_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_bipartite(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Edge incidence>

  The command <verbatim|incident_edges><index|<verbatim|incident_edges>> is
  used for obtaining edges incident to a given vertex in a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|indcident_edges(G,v)>>>|<row|<cell|>|<cell|<verbatim|indcident_edges(G,L)>>>>>>

  <verbatim|incident_edges> takes two argument, a graph
  <math|G<around*|(|V,E|)>> and a vertex <math|v\<in\>V> or a list of
  vertices <math|L\<subset\>V>. The command returns the list of edges
  <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|k>\<in\>E> which have <math|v>
  as one of its endpoints.

  Note that edge directions are ignored when <math|G> is a digraph. To obtain
  only outgoing or incoming edges, use the commands
  <verbatim|<hlink|departures|#departures-arrivals>> and
  <verbatim|<hlink|arrivals|#departures-arrivals>>, respectively.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=cycle_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incident_edges(G,1)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|1>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incident_edges(G,[2,4,5])
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|1>|<cell|5>>|<row|<cell|2>|<cell|3>>|<row|<cell|3>|<cell|4>>|<row|<cell|4>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tournament([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incident_edges(G,2)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|2>|<cell|3>>|<row|<cell|2>|<cell|5>>|<row|<cell|4>|<cell|2>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <section|Algebraic properties>

  <subsection|Adjacency matrix>

  The command <verbatim|adjacency_matrix><index|<verbatim|adjacency_matrix>>
  is used for obtaining the <hlink|<rigid|adjacency
  matrix>|https://en.wikipedia.org/wiki/Adjacency_matrix> of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|adjacency_matrix(G)>>>>>>

  <verbatim|adjacency_matrix> takes a graph <math|G<around*|(|V,E|)>>, where
  <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>, as its only
  argument and returns the square matrix <math|A=<around*|[|a<rsub|i j>|]>>
  of order <math|n> such that, for <math|i,j=1,2,\<ldots\>,n>,

  <\equation*>
    a<rsub|i j>=<choice|<tformat|<table|<row|<cell|1\<comma\>>|<cell|if
    <text|the set <math|E> contains edge/arc <math|v<rsub|i>
    v<rsub|j>>,>>>|<row|<cell|0,>|<cell|<text|otherwise>.>>>>>
  </equation*>

  Note that <math|tr <around*|(|A|)>=0>. Also, the adjacency matrix of an
  undirected graph is always symmetrical.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=adjacency_matrix(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|6|6|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|1>|<cell|1>|<cell|1>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|1>|<cell|1>|<cell|0>|<cell|1>>|<row|<cell|1>|<cell|1>|<cell|0>|<cell|0>|<cell|1>|<cell|1>>|<row|<cell|1>|<cell|1>|<cell|0>|<cell|0>|<cell|1>|<cell|1>>|<row|<cell|1>|<cell|0>|<cell|1>|<cell|1>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|1>|<cell|1>|<cell|1>|<cell|1>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      transpose(A)==A
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D:=digraph(trail(1,2,3,4,5,2,6,7,3,8,1))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 8 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D)
    </input>
  </session>

  <center|<image|images/digraph8.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=adjacency_matrix(D)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|7|7|cell-halign|c>|<cwith|1|-1|8|8|cell-halign|c>|<cwith|1|-1|8|8|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      transpose(A)==A
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Laplacian matrix><label|laplacian-matrix>

  The command <verbatim|laplacian_matrix><index|<verbatim|laplacian_matrix>>
  is used for computing the <hlink|<rigid|Laplacian
  matrix>|https://en.wikipedia.org/wiki/Laplacian_matrix#Laplacian_matrix_for_simple_graphs>
  of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|laplacian_matrix(G)>>>|<row|<cell|>|<cell|<verbatim|laplacian_matrix(G,normal)>>>>>>

  <verbatim|laplacian_matrix> takes an undirected graph
  <math|G<around*|(|V,E|)>>, where <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>,
  and returns the symmetric matrix <math|L=D-A>, where <math|A> is the
  adjacency matrix of <math|G> and

  <\equation*>
    D=<matrix|<tformat|<table|<row|<cell|deg<around*|(|v<rsub|1>|)>>|<cell|0>|<cell|0>|<cell|\<cdots\>>|<cell|0>>|<row|<cell|0>|<cell|deg<around*|(|v<rsub|2>|)>>|<cell|0>|<cell|\<cdots\>>|<cell|0>>|<row|<cell|\<vdots\>>|<cell|\<vdots\>>|<cell|\<vdots\>>|<cell|\<ddots\>>|<cell|\<vdots\>>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|\<cdots\>>|<cell|deg<around*|(|v<rsub|n>|)>>>>>>.
  </equation*>

  The option <verbatim|normal> may be passed as the second argument. In that
  case, the <hlink|<rigid|normalized Laplacian>|https://en.wikipedia.org/wiki/Laplacian_matrix#Symmetric_normalized_Laplacian_2>
  <math|L<rsup|sym>\<assign\>I-D<rsup|-1/2>*A*D<rsup|-1/2>> of <math|G> is
  returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=path_graph(4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=adjacency_matrix(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|4|4|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L:=laplacian_matrix(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|4|4|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|-1>|<cell|0>|<cell|0>>|<row|<cell|-1>|<cell|2>|<cell|-1>|<cell|0>>|<row|<cell|0>|<cell|-1>|<cell|2>|<cell|-1>>|<row|<cell|0>|<cell|0>|<cell|-1>|<cell|1>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      diag(degree_sequence(G))-A==L
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      laplacian_matrix(G,normal)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|4|4|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|<frac|-1|<sqrt|2>>>|<cell|0>|<cell|0>>|<row|<cell|<frac|-1|<sqrt|2>>>|<cell|1>|<cell|<frac|-1|2>>|<cell|0>>|<row|<cell|0>|<cell|<frac|-1|2>>|<cell|1>|<cell|<frac|-1|<sqrt|2>>>>|<row|<cell|0>|<cell|0>|<cell|<frac|-1|<sqrt|2>>>|<cell|1>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  The smallest eigenvalue of a Laplacian matrix of an undirected graph is
  always zero. Moreover, its multiplicity is equal to the number of connected
  components in the corresponding graph <cite-detail|godsil|pp.<nbsp>280>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      sort(eigenvals(L))
    <|unfolded-io>
      <\equation*>
        0,-<sqrt|2>+2,2,<sqrt|2>+2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=disjoint_union(complete_graph(4),cycle_graph(3),path_graph(2))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 9 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,labels=false)
    </input>
  </session>

  <center|<image|images/disjoint2.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      eigenvals(laplacian_matrix(H))
    <|unfolded-io>
      <\equation*>
        0,0,0,4,4,4,3,3,2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      nops(connected_components(H))
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Incidence matrix>

  The command <verbatim|incidence_matrix><index|<verbatim|incidence_matrix>>
  is used for obtaining the <hlink|<rigid|incidence
  matrix>|https://en.wikipedia.org/wiki/Incidence_matrix> of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|incidence_matrix(G)>>>>>>

  <verbatim|incidence_matrix> takes a graph <math|G<around*|(|V,E|)>>, where
  <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>> and
  <math|E=<around*|{|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|m>|}>>, as its only
  argument and returns the <math|n\<times\>m> matrix
  <math|B=<around*|[|b<rsub|i j>|]>> such that, for all
  <math|i=1,2,\<ldots\>,n> and <math|j=1,2,\<ldots\>,m>,

  <\equation*>
    b<rsub|i j>=<choice|<tformat|<table|<row|<cell|1,>|<cell|<text|if the
    vertex <math|v<rsub|i>> is incident to the edge
    <math|e<rsub|j>>,>>>|<row|<cell|0,>|<cell|<text|otherwise>>>>>>
  </equation*>

  if <math|G> is undirected resp.

  <\equation*>
    b<rsub|i j>=<choice|<tformat|<cwith|1|-1|1|1|cell-halign|r>|<table|<row|<cell|1,>|<cell|<text|if
    the vertex <math|v<rsub|i>> is the head of the arc
    <math|e<rsub|j>>,>>>|<row|<cell|-1,>|<cell|<text|if the vertex
    <math|v<rsub|i>> is the tail of the arc
    <math|e<rsub|j>>,>>>|<row|<cell|0,>|<cell|<text|otherwise>>>>>>
  </equation*>

  if <math|G> is directed.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K4:=complete_graph([1,2,3,4])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(K4)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|1>|<cell|3>>|<row|<cell|1>|<cell|4>>|<row|<cell|2>|<cell|3>>|<row|<cell|2>|<cell|4>>|<row|<cell|3>|<cell|4>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incidence_matrix(K4)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|6|6|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|1>|<cell|1>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|0>|<cell|1>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|1>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|1>|<cell|1>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      DG:=digraph(trail(1,2,3,4,5,3),trail(1,5,2,4,1))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 9 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(DG)
    </input>
  </session>

  <center|<image|images/digraph9.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(DG)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|1>|<cell|5>>|<row|<cell|2>|<cell|3>>|<row|<cell|2>|<cell|4>>|<row|<cell|3>|<cell|4>>|<row|<cell|4>|<cell|1>>|<row|<cell|4>|<cell|5>>|<row|<cell|5>|<cell|2>>|<row|<cell|5>|<cell|3>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incidence_matrix(DG)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|7|7|cell-halign|c>|<cwith|1|-1|8|8|cell-halign|c>|<cwith|1|-1|9|9|cell-halign|c>|<cwith|1|-1|9|9|cell-rborder|0ln>|<table|<row|<cell|-1>|<cell|-1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|-1>|<cell|-1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|-1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|1>|<cell|-1>|<cell|-1>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|-1>|<cell|-1>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Weight matrix>

  The command <verbatim|weight_matrix><index|<verbatim|weight_matrix>> is
  used for obtaining the weight matrix of a <hlink|<rigid|weighted
  graph>|https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Weighted_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|weight_matrix(G)>>>>>>

  <verbatim|weight_matrix> takes a graph <math|G<around*|(|V,E|)>>, where
  <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>, as its only
  argument and returns the square matrix <math|M=<around*|[|m<rsub|i j>|]>>
  of order <math|n> such that <math|m<rsub|i j>> equals zero if
  <math|v<rsub|i>> and <math|v<rsub|j>> are not adjacent and the weight of
  the edge/arc <math|v<rsub|i> v<rsub|j>> otherwise, for all
  <math|i,j=1,2,\<ldots\>,n> (note that the weight of an edge/arc may be any
  real number).

  Note that <math|tr <around*|(|M|)>=0>. Also, the weight matrix of an
  undirected graph is always symmetrical.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[[1,2],1],[[2,3],2],[[4,5],3],[[5,2],4]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      weight_matrix(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|5|5|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|1>|<cell|0>|<cell|2>|<cell|0>|<cell|4>>|<row|<cell|0>|<cell|2>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|3>>|<row|<cell|0>|<cell|4>|<cell|0>|<cell|3>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Characteristic polynomial>

  The command <verbatim|graph_charpoly><index|<verbatim|graph_charpoly>> or
  <verbatim|charpoly> is used for obtaining the <hlink|<rigid|characteristic
  polynomial>|https://en.wikipedia.org/wiki/Characteristic_polynomial> of an
  undirected graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_charpoly(G)>>>|<row|<cell|>|<cell|<verbatim|graph_charpoly(G,x)>>>|<row|<cell|>|<cell|<verbatim|charpoly(G)>>>|<row|<cell|>|<cell|<verbatim|charpoly(G,x)>>>>>>

  <verbatim|graph_charpoly> or <verbatim|charpoly> takes one or two
  arguments, an undirected graph <math|G<around*|(|V,E|)>> and optionally a
  value or symbol <math|x>. The command returns <math|p<around*|(|x|)>>,
  where <math|p> is the characteristic polynomial of the adjacency matrix of
  <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      charpoly(G,x)
    <|unfolded-io>
      <\equation*>
        x<rsup|3>-2*x
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      charpoly(G,3)
    <|unfolded-io>
      <\equation*>
        21
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("shrikhande")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 48 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      charpoly(G,x)
    <|unfolded-io>
      <\equation*>
        x<rsup|16>-48*x<rsup|14>-64*x<rsup|13>+768*x<rsup|12>+1536*x<rsup|11>-5888*x<rsup|10>-15360*x<rsup|9>+23040*x<rsup|8>+81920*x<rsup|7>-36864*x<rsup|6>-245760*x<rsup|5>-32768*x<rsup|4>+393216*x<rsup|3>+196608*x<rsup|2>-262144*x-196608
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Graph spectrum><label|graph-spectrum>

  The command <verbatim|graph_spectrum><index|<verbatim|graph_spectrum>> is
  used for computing <hlink|<rigid|graph spectra>|https://en.wikipedia.org/wiki/Adjacency_matrix#Spectrum>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_spectrum(G)>>>>>>

  <verbatim|graph_spectrum> takes a graph <math|G> as its only argument and
  returns the list in which every element is an eigenvalue of the adjacency
  matrix of <math|G> paired with its multiplicity.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C5:=cycle_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      gs:=graph_spectrum(C5)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|1>>|<row|<cell|<frac|<sqrt|5>-1|2>>|<cell|2>>|<row|<cell|<frac|-<sqrt|5>-1|2>>|<cell|2>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      p:=charpoly(C5,x)
    <|unfolded-io>
      <\equation*>
        x<rsup|5>-5*x<rsup|3>+5*x-2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      expand(roots(p))==expand(gs)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  The above result indicates that <verbatim|gs> and <verbatim|roots(p)> are
  equal.

  <subsection|Seidel spectrum>

  The command <verbatim|seidel_spectrum><index|<verbatim|seidel_spectrum>> is
  used for computing <hlink|<rigid|Seidel
  spectra>|https://en.wikipedia.org/wiki/Seidel_adjacency_matrix>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|seidel_spectrum(G)>>>>>>

  <verbatim|seidel_spectrum> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns the list in which every element is an eigenvalue
  of the <hlink|<rigid|Seidel adjacency matrix>|https://en.wikipedia.org/wiki/Seidel_adjacency_matrix>
  <math|S> paired with its multiplicity. The matrix <math|S>, which can be
  interpreted as the difference of the adjacency matrices of <math|G> and its
  complement <math|G<rsup|c>>, is computed as <math|J-I-2*A>, where <math|J>
  is all-one <math|n\<times\>n> matrix, <math|I> is the identity matrix of
  order <math|n>, <math|A> is the adjacency matrix of <math|G> and
  <math|n=<around*|\||V|\|>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      seidel_spectrum(graph("clebsch"))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|-3>|<cell|10>>|<row|<cell|5>|<cell|6>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      seidel_spectrum(graph("levi"))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|-5>|<cell|9>>|<row|<cell|-1>|<cell|10>>|<row|<cell|3>|<cell|9>>|<row|<cell|5>|<cell|1>>|<row|<cell|23>|<cell|1>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Integer graphs>

  The command <verbatim|is_integer_graph><index|<verbatim|is_integer_graph>>
  is used for determining whether a graph is an <hlink|<rigid|integral
  graph>|https://en.wikipedia.org/wiki/Integral_graph>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_integer_graph(G)>>>>>>

  <verbatim|is_integer_graph> takes a graph <math|G> as its only argument and
  returns <verbatim|true> if the <hlink|<rigid|spectrum>|#graph-spectrum> of
  <math|G> consists only of integers. Else it returns <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("levi")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 45 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_integer_graph(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      factor(charpoly(G,x))
    <|unfolded-io>
      <\equation*>
        x<rsup|10>*<around|(|x-3|)>*<around*|(|x-2|)><rsup|9>*<around*|(|x+2|)><rsup|9>*<around|(|x+3|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_spectrum(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|-3>|<cell|1>>|<row|<cell|-2>|<cell|9>>|<row|<cell|0>|<cell|10>>|<row|<cell|2>|<cell|9>>|<row|<cell|3>|<cell|1>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <section|Graph isomorphism>

  <subsection|Isomorphic graphs><label|is-isomorphic>

  The command <verbatim|is_isomorphic><index|<verbatim|is_isomorphic>> is
  used for determining whether two graphs are
  <hlink|isomorphic|https://en.wikipedia.org/wiki/Graph_isomorphism>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_isomorphic(G1,G2)>>>|<row|<cell|>|<cell|<verbatim|is_isomorphic(G1,G2,m)>>>|<row|<cell|>|<cell|<verbatim|canonical_labeling(G)>>>>>>

  <verbatim|is_isomorphic> takes two or three arguments: a graph
  <math|G<rsub|1><around*|(|V<rsub|1>,E<rsub|1>|)>>, a graph
  <math|G<rsub|2><around*|(|V<rsub|2>,E<rsub|2>|)>> and optionally an
  unassigned identifier <verbatim|m>. The command returns <verbatim|true> if
  <math|G<rsub|1>> and <math|G<rsub|2>> are isomorphic and <verbatim|false>
  otherwise. If the third argument is given and <math|G<rsub|1>> and
  <math|G<rsub|2>> are isomorphic, the list of pairwise matching of vertices
  in <math|G<rsub|1>> and <math|G<rsub|2>>, representing the isomorphism
  between the two graphs, is stored to <verbatim|m>.

  Note that the algorithm takes vertex colors into account. Namely, only
  vertices sharing the same color can be mapped to each other. Vertex colors
  can be set by calling the <verbatim|<hlink|highlight_vertex|#highlight-vertex>>
  command.

  This command, as well as the commands <verbatim|<hlink|canonical_labeling|#canonical-labeling>>
  and <verbatim|<hlink|graph_automorphisms|#automorphisms>> described later
  in this section, is using <hlink|<samp|nauty>|http://pallini.di.uniroma1.it/>
  library developed by <name|Brendan McKay> <cite|mckay>, which is one of the
  fastest implementations for graph isomorphism.

  For example, entering the command line below one shows that Petersen graph
  is isomorphic to Kneser graph <math|K<around*|(|5,2|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(graph("petersen"),kneser_graph(5,2))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  In the following example, <math|G<rsub|1>> and <math|G<rsub|3>> are
  isomorphic while <math|G<rsub|1>> and <math|G<rsub|2>> are not isomorphic.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph(trail(1,2,3,4,5,6,1,3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=graph(trail(1,2,3,4,5,6,1,4))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G3:=graph(trail(1,2,3,4,5,6,1,5))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G1,circle)
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G2,circle)
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G3,circle)
    </input>
  </session>

  The drawings are ordered from left to right.

  <htab|5mm><image|images/isom1.eps|35%|||><htab|5mm><image|images/isom2.eps|35%|||><htab|5mm><image|images/isom3.eps|35%|||><htab|5mm>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G1,G3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G1,G3,mapping):; mapping
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<around|[|1=5,2=6,3=1,4=2,5=3,6=4|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H1:=highlight_vertex(G1,5):; H3:=highlight_vertex(G3,5):;
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(H1,H3)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H1:=highlight_vertex(H1,1):; H3:=highlight_vertex(H3,3):;
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(H1,H3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  In the next example, <math|D<rsub|1>> and <math|D<rsub|3>> are isomorphic
  while <math|D<rsub|1>> and <math|D<rsub|2>> are not isomorphic.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D1:=digraph(trail(1,2,3,1,4,5))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 5 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D2:=digraph(trail(1,2,3,4,5,3))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 5 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      D3:=digraph([1,2,3,4,5],trail(3,4,5,3,1,2))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 5 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D1,circle)
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D2,circle)
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D3,circle)
    </input>
  </session>

  The drawings are ordered from left to right.

  <htab|5mm><image|images/isom4.eps|35%|||><htab|5mm><image|images/isom5.eps|35%|||><htab|5mm><image|images/isom6.eps|35%|||><htab|5mm>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(D1,D2)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(D1,D3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  Isomorphism testing with <samp|nauty> is very fast and can be used for
  large graphs, as in the example below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10000,0.01)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 499867
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=isomorphic_copy(G,randperm(10000))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 499867
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G,H)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>

      <timing|1.7 sec>
    </unfolded-io>
  </session>

  To make the edge structures of <math|G> and <math|H> slightly different, a
  random edge from <math|H> is \Pmisplaced\Q.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      ed:=edges(H)[rand(number_of_edges(H))]
    <|unfolded-io>
      <\equation*>
        <around|[|813,3021|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      has_edge(H,[813,3022])
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=add_edge(delete_edge(H,ed),[813,3022])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 499867
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G,H)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Canonical labeling><label|canonical-labeling>

  Graph isomorphism testing in <samp|nauty> is based on computing the
  canonical labelings for the input graphs. The <strong|canonical labeling>
  of <math|G> is a particular ordering of the vertices of <math|G>.
  Rearranging the vertices with respect to that ordering produces the
  <strong|canonical representation> of <math|G>. Two graphs are isomorphic if
  and only if their canonical representations share the same edge structure.

  The command <verbatim|canonical_labeling><index|<verbatim|canonical_labeling>>
  is used for computing the canonical labeling as a permutation. One can
  reorder the vertices by using this permutation with the
  <verbatim|<hlink|isomorphic_copy|#isomorphic-copy>> command.\ 

  <verbatim|canonical_labeling> takes a graph <math|G<around*|(|V,E|)>> as
  its only argument and returns the permutation representing the canonical
  labeling of <math|G>. Note that the colors of the vertices are taken into
  account.

  In the next example it is demonstrated how to prove that <math|G<rsub|1>>
  and <math|G<rsub|3>> are isomorphic by comparing their canonical
  representations <math|C<rsub|1>> and <math|C<rsub|3>> with the
  <verbatim|<hlink|graph_equal|#graph-equal>> command. Before testing
  <math|C<rsub|1>> and <math|C<rsub|3>> for equality, their vertices have to
  be relabeled so that the command <verbatim|<hlink|vertices|#vertices-edges>>
  gives the same result for both graphs.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L1:=canonical_labeling(G1)
    <|unfolded-io>
      <\equation*>
        <around|[|4,3,5,1,2,0|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L3:=canonical_labeling(G3)
    <|unfolded-io>
      <\equation*>
        <around|[|2,1,3,5,0,4|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C1:=relabel_vertices(isomorphic_copy(G1,L1),[1,2,3,4,5,6])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C3:=relabel_vertices(isomorphic_copy(G3,L3),[1,2,3,4,5,6])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_equal(C1,C3)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Graph automorphisms><label|automorphisms>

  The command <verbatim|graph_automorphisms><index|<verbatim|graph_automorphisms>>
  is used for finding generators of the <hlink|automorphism|https://en.wikipedia.org/wiki/Graph_automorphism>
  group of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_automorphisms(G)>>>>>>

  <verbatim|graph_automorphisms> takes a graph <math|G> as its only argument
  and returns a list containing the generators of <math|Aut<around*|(|G|)>>,
  the automorphism group of <math|G> (see <cite-detail|godsil|pp.<nbsp>4> and
  <cite-detail|biggs|pp.<nbsp>115>). Each generator is given as a list of
  cycles, which can be turned to a permutation by calling the command
  <verbatim|cycles2permu>.

  Note that vertex colors are taken into account. Only vertices sharing the
  same color can be mapped to each other. The color of a vertex can be set by
  calling the command <verbatim|<hlink|highlight_vertex|#highlight-vertex>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      g:=graph_automorphisms(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        <around|{|<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|3>|<cell|7>>|<row|<cell|4>|<cell|5>>|<row|<cell|8>|<cell|9>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|6>>|<row|<cell|3>|<cell|8>>|<row|<cell|4>|<cell|5>>|<row|<cell|7>|<cell|9>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|4>>|<row|<cell|2>|<cell|3>>|<row|<cell|6>|<cell|9>>|<row|<cell|7>|<cell|8>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|2>|<cell|4>>|<row|<cell|5>|<cell|6>>|<row|<cell|7>|<cell|9>>>>>|)>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cycles2permu(g[2])
    <|unfolded-io>
      <\equation*>
        <around|[|0,4,3,2,1,5,9,8,7,6|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=highlight_vertex(G,4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_automorphisms(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|6>>|<row|<cell|3>|<cell|9>>|<row|<cell|7>|<cell|8>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|5>>|<row|<cell|2>|<cell|7>>|<row|<cell|3>|<cell|9>>|<row|<cell|6>|<cell|8>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|3>>|<row|<cell|1>|<cell|2>>|<row|<cell|5>|<cell|8>>|<row|<cell|6>|<cell|7>>>>>|)>|}>
      </equation*>
    </unfolded-io>
  </session>

  In the above result, all permutations map the vertex 4 to itself, because
  it is the single green-colored vertex in <math|G> (it cannot be mapped to
  any other vertex because colors do not match).

  Frucht graph (see the page<nbsp><pageref|frucht>) is an example of a graph
  with automorphism group containing only the identity, so the set of its
  generators is empty:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_automorphisms(graph("frucht"))
    <|unfolded-io>
      <\equation*>
        <around|{||}>
      </equation*>
    </unfolded-io>
  </session>

  <section|Graph polynomials>

  <subsection|Tutte polynomial><label|tutte-polynomial>

  The command <verbatim|tutte_polynomial><index|<verbatim|tutte_polynomial>>
  is used for computing <hlink|<rigid|Tutte
  polynomials>|https://en.wikipedia.org/wiki/Tutte_polynomial>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|tutte_polynomial(G)>>>|<row|<cell|>|<cell|<verbatim|tutte_polynomial(G,x,y)>>>>>>

  <verbatim|tutte_polynomial> takes one or three arguments, an undirected
  graph <math|G<around*|(|V,E|)>> and optionally two variables or values
  <math|x> and <math|y>. It returns the the bivariate Tutte
  polynomial<\footnote>
    See <cite|haggard>, <cite-detail|biggs|pp.<nbsp>97> and
    <cite-detail|bollobas|pp.<nbsp>335>.
  </footnote> <math|T<rsub|G>> of <math|G> or the value
  <math|T<rsub|G><around*|(|x,y|)>> if the optional arguments are given. If
  <math|G> is weighted, it is treated as a multigraph: the weight <math|w> of
  an edge <math|e>, which must be a positive integer, is interpreted as the
  multiplicity of <math|e>, for each <math|e\<in\>E>. Note, however, that
  loops are not supported.

  The strategy is to apply the recursive definition of Tutte polynomial
  <cite|haggard> together with the <samp|vorder> heuristic proposed by
  <name|Haggard> et al.<nbsp><cite|haggard2> and improved by <name|Monagan>
  <cite|monagan>. The subgraphs appearing in the computation tree are cached
  and reused when possible, pruning the tree significantly. Subgraphs are
  stored (and compared) in their canonical form, for which the <samp|nauty>
  library is used.

  Note that finding Tutte polynomials is <abbr|NP>-hard in general, hence the
  problem becomes intractable for larger and/or denser graphs.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K4:=complete_graph(4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tutte_polynomial(K4,x,y)
    <|unfolded-io>
      <\equation*>
        x<rsup|3>+3*x<rsup|2>+4*x*y+2*x+y<rsup|3>+3*y<rsup|2>+2*y
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tutte_polynomial(K4,x,1)
    <|unfolded-io>
      <\equation*>
        x<rsup|3>+3*x<rsup|2>+6*x+6
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      f:=tutte_polynomial(G)
    <|unfolded-io>
      <\equation*>
        x<rsup|9>+6*x<rsup|8>+21*x<rsup|7>+56*x<rsup|6>+12*x<rsup|5>*y+114*x<rsup|5>+70*x<rsup|4>*y+170*x<rsup|4>+30*x<rsup|3>*y<rsup|2>+170*x<rsup|3>*y+180*x<rsup|3>+15*x<rsup|2>*y<rsup|3>+105*x<rsup|2>*y<rsup|2>+240*x<rsup|2>*y+120*x<rsup|2>+10*x*y<rsup|4>+65*x*y<rsup|3>+171*x*y<rsup|2>+168*x*y+36*x+y<rsup|6>+9*y<rsup|5>+35*y<rsup|4>+75*y<rsup|3>+84*y<rsup|2>+36*y
      </equation*>
    </unfolded-io>
  </session>

  This result coincides with that in <cite-detail|biggs|pp.<nbsp>103>, which
  is supposed to be correct. Alternatively, it can be verified by applying
  the recursive definition with an arbitrary edge <math|e\<in\>E>, as below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      ed:=edges(G)[0]
    <|unfolded-io>
      <\equation*>
        <around|[|0,1|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Gdelete:=delete_edge(G,ed)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Gcontract:=contract_edge(G,ed)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 9 vertices and 14 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      expand(f-tutte_polynomial(Gdelete)-tutte_polynomial(Gcontract))
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>
  </session>

  The value <math|T<rsub|G><around*|(|1,1|)>> is equal to the number of
  spanning forests in <math|G> <cite-detail|bollobas|pp.<nbsp>345>\Vin this
  case, the number of spanning trees in Petersen graph. For verification, the
  same number is computed by using the specialized command
  <hlink|<verbatim|number_of_spanning_trees>|#number-of-spanning-trees>,
  which uses a different (much faster) algorithm.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tutte_polynomial(G,1,1)
    <|unfolded-io>
      <\equation*>
        2000
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_spanning_trees(G)
    <|unfolded-io>
      <\equation*>
        2000
      </equation*>
    </unfolded-io>
  </session>

  For a graph <math|G> and its dual <math|G<rsup|\<ast\>><rsup|>> the
  following relation holds: <math|T<rsub|G><around*|(|x,y|)>=T<rsub|G<rsup|\<ast\>>><around*|(|y,x|)>>.
  Therefore, if <math|T<rsub|G><around*|(|x,y|)>=T<rsub|G><around*|(|y,x|)>>
  then <math|G> and <math|G<rsup|\<ast\>>> are isomorphic (since Tutte
  polynomial is a graph invariant). A simple example of such graph is
  tetrahedral graph. Since it is planar and biconnected, its dual can be
  determined by using the command <verbatim|plane_dual>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("tetrahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(G) and is_planar(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=plane_dual(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=tutte_polynomial(G)
    <|unfolded-io>
      <\equation*>
        x<rsup|3>+3*x<rsup|2>+4*x*y+2*x+y<rsup|3>+3*y<rsup|2>+2*y
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      expand(T-subs(T,[x,y],[y,x]))
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G,H)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  Multiple edges can be specified as edge weights.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=make_weighted(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=set_edge_weight(set_edge_weight(M,[1,2],2),[3,4],3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(M,weights)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|<around|[|1,2|]>,2|]>,<around|[|<around|[|1,3|]>,1|]>,<around|[|<around|[|1,4|]>,1|]>,<around|[|<around|[|2,3|]>,1|]>,<around|[|<around|[|2,4|]>,1|]>,<around|[|<around|[|3,4|]>,3|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tutte_polynomial(M,x,y)
    <|unfolded-io>
      <\equation*>
        x<rsup|3>+x<rsup|2>*y<rsup|2>+2*x<rsup|2>*y+3*x<rsup|2>+3*x*y<rsup|3>+6*x*y<rsup|2>+6*x*y+2*x+y<rsup|6>+3*y<rsup|5>+6*y<rsup|4>+7*y<rsup|3>+5*y<rsup|2>+2*y
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Chromatic polynomial>

  The command <verbatim|chromatic_polynomial><index|<verbatim|chromatic_polynomial>>,
  is used for computing <hlink|<rigid|chromatic
  polynomials>|https://en.wikipedia.org/wiki/Tutte_polynomial#Chromatic_polynomial>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|chromatic_polynomial(G)>>>|<row|<cell|>|<cell|<verbatim|chromatic_polynomial(G,t)>>>>>>

  <verbatim|chromatic_polynomial> takes one or two arguments, an undirected
  unweighted graph <math|G<around*|(|V,E|)>> and optionally a variable or
  value <math|t>. It returns the chromatic polynomial <math|P<rsub|G>> of
  <math|G> or the value <math|P<rsub|G><around*|(|t|)>> if the second
  argument is given.

  <math|P<rsub|G>> and the <hlink|Tutte polynomial|#tutte-polynomial>
  <math|T<rsub|G>> satisfy the following relation<nbsp>(see <cite|haggard>
  and <cite-detail|bollobas|pp.<nbsp>346>):

  <\equation>
    P<rsub|G><around*|(|t|)>=<around*|(|-1|)><rsup|<around*|\||V|\|>-\<kappa\><around*|(|G|)>>*t<rsup|\<kappa\><around*|(|G|)>>*T<rsub|G><around*|(|1-t,0|)>,<label|eq:chromatic-poly>
  </equation>

  where <math|\<kappa\><around*|(|G|)>> is the number of connected components
  of <math|G>. <verbatim|chromatic_polynomial>
  uses<nbsp><eqref|eq:chromatic-poly> to compute <math|P<rsub|G>>.

  The value <math|P<rsub|G><around*|(|k|)>>, where <math|k\<gtr\>0> is an
  integer, is equal to the number of all distinct <math|k>-colorings of
  vertices in <math|G>. As shown in the example below, Petersen graph cannot
  be colored by using only two colors, but is 3-colorable with 120 distinct
  colorings (all using the same three colors).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=chromatic_polynomial(graph("petersen"),x)
    <|unfolded-io>
      <\equation*>
        x*<around|(|x-2|)>*<around|(|x-1|)>*<around|(|x<rsup|7>-12*x<rsup|6>+67*x<rsup|5>-230*x<rsup|4>+529*x<rsup|3>-814*x<rsup|2>+775*x-352|)>
      </equation*>
    </unfolded-io>
  </session>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      subs(P,x=2)
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      subs(P,x=3)
    <|unfolded-io>
      <\equation*>
        120
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Flow polynomial>

  The command <verbatim|flow_polynomial><index|<verbatim|flow_polynomial>> is
  used for computing <hlink|<rigid|flow polynomials>|https://en.wikipedia.org/wiki/Tutte_polynomial#Flow_polynomial>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|flow_polynomial(G)>>>|<row|<cell|>|<cell|<verbatim|flow_polynomial(G,x)>>>>>>

  <verbatim|flow_polynomial> takes one or two arguments, an undirected
  unweighted graph <math|G<around*|(|V,E|)>> and optionally a variable or
  value <math|x>. It returns the flow polynomial <math|Q<rsub|G>> of <math|G>
  or the value <math|Q<rsub|G><around*|(|x|)>> if the second argument is
  given.

  <math|Q<rsub|G>> and the <hlink|Tutte polynomial|#tutte-polynomial>
  <math|T<rsub|G>> satisfy the following relation<nbsp>(see <cite|haggard>
  and <cite-detail|biggs|pp.<nbsp>110>):

  <\equation>
    Q<rsub|G><around*|(|x|)>=<around*|(|-1|)><rsup|<around*|\||E|\|>-<around*|\||V|\|>+\<kappa\><around*|(|G|)>>*T<rsub|G><around*|(|0,1-x|)>,<label|eq:flow-poly>
  </equation>

  where <math|\<kappa\><around*|(|G|)>> is the number of connected components
  of <math|G>. <verbatim|flow_polynomial> uses<nbsp><eqref|eq:flow-poly> to
  compute <math|Q<rsub|G>>.

  The value <math|Q<rsub|G><around*|(|k|)>>, where <math|k\<gtr\>0> is an
  integer, is equal to the number of all nowhere-zero <math|k>-flows in
  <math|G>. In such flows, the total flow <math|f<rsub|v>> entering and
  leaving vertex <math|v> is congruent modulo <math|k>, hence
  <math|f<rsub|v>\<in\><around*|{|1,2,\<ldots\>,k-1|}>> for all
  <math|v\<in\>V> <cite-detail|bollobas|pp.<nbsp>347>. As shown in the
  example below, Petersen graph has zero 4-flows and 240 5-flows.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Q:=flow_polynomial(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        x<rsup|6>-15*x<rsup|5>+95*x<rsup|4>-325*x<rsup|3>+624*x<rsup|2>-620*x+240
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Q \| x=4
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Q \| x=5
    <|unfolded-io>
      <\equation*>
        240
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Reliability polynomial>

  The command <verbatim|reliability_polynomial><index|<verbatim|reliability_polynomial>>
  is used for computing <hlink|<rigid|reliability
  polynomials>|https://en.wikipedia.org/wiki/Tutte_polynomial#Reliability_polynomial>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|reliability_polynomial(G)>>>|<row|<cell|>|<cell|<verbatim|reliability_polynomial(G,p)>>>>>>

  <verbatim|reliability_polynomial> takes one or two arguments, an undirected
  graph <math|G<around*|(|V,E|)>> and optionally a variable or value
  <math|p>. It returns the all-terminal reliability polynomial
  <math|R<rsub|G>> of <math|G> or the value <math|R<rsub|G><around*|(|p|)>>
  if the second argument is given. If <math|G> is weighted, it is treated as
  a multigraph: the weight <math|w> of an edge <math|e>, which must be a
  positive integer, is interpreted as the multiplicity of <math|e>, for each
  <math|e\<in\>E>.

  <math|R<rsub|G>> and the <hlink|Tutte polynomial|#tutte-polynomial>
  <math|T<rsub|G>> satisfy the following relation <cite|monagan>:

  <\equation>
    R<rsub|G><around*|(|p|)>=<around*|(|1-p|)><rsup|<around*|\||V|\|>-\<kappa\><around*|(|G|)>>*p<rsup|<around*|\||E|\|>-<around*|\||V|\|>+\<kappa\><around*|(|G|)>>*T<rsub|G><around*|(|1,p<rsup|-1>|)>,<label|eq:reliability-poly>
  </equation>

  where <math|\<kappa\><around*|(|G|)>> is the number of connected components
  of <math|G>. <verbatim|reliability_polynomial>
  uses<nbsp><eqref|eq:reliability-poly> to compute <math|R<rsub|G>>.

  If <math|G> is a connected network, then the value
  <math|R<rsub|G><around*|(|p|)>>, where <math|p\<in\><around*|[|0,1|]>>, is
  equal to the probability that <math|G> does not fail (i.e.<nbsp>stays
  connected) after removing each edge with probability
  <math|p><nbsp><cite-detail|godsil|pp.<nbsp>354\U355>.

  In the following example, it is clear that the graph <math|G> will stay
  connected with probability <math|<around*|(|1-p|)><rsup|2>> if each of its
  two edges is removed with probability <math|p>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 2 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      R:=reliability_polynomial(G,p)
    <|unfolded-io>
      <\equation*>
        p<rsup|2>-2*p+1
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      factor(R)
    <|unfolded-io>
      <\equation*>
        <around*|(|p-1|)><rsup|2>
      </equation*>
    </unfolded-io>
  </session>

  Adding a new edge should increase the reliability of <math|G>, since the
  latter is connected. Indeed, the difference <math|S-R> below is positive
  for <math|0\<less\>p\<less\>1>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=reliability_polynomial(add_edge(G,[1,3]),p)
    <|unfolded-io>
      <\equation*>
        2*p<rsup|3>-3*p<rsup|2>+1
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      factor(S-R)
    <|unfolded-io>
      <\equation*>
        2*p*<around*|(|p-1|)><rsup|2>
      </equation*>
    </unfolded-io>
  </session>

  Multiple edges can be specified as edge weights.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=graph(%{[[1,2],2],[[2,3],1],[[3,1],1]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      factor(reliability_polynomial(M))
    <|unfolded-io>
      <\equation*>
        <around*|(|x-1|)><rsup|2>*<around|(|2*x<rsup|2>+2*x+1|)>
      </equation*>
    </unfolded-io>
  </session>

  The following graph represents the Arpanet (early internet) in December
  1970.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      V:=["MIT","LINCOLN","CASE","CMU","HARVARD","BBN","UCSB","UCLA","STANFORD","SRI","RAND","UTAH","SDC"]:;
    </input>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=graph(V,trail("BBN","HARVARD","CMU","CASE","LINCOLN","MIT","UTAH","SRI","STANFORD","UCLA","UCSB","SRI","UCLA","RAND","BBN","MIT"),trail("RAND","SDC","UTAH"))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 13 vertices and 17 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      Arpanet:=set_vertex_positions(A,[[1.0,1.0],[0.9,1.2],[0.5,1.1],[0.6,0.8],[1.0,0.6],[1.0,0.8],[-1.1,0.1],[-0.8,0.3],[-0.6,0.5],[-0.8,0.7],[-0.8,-0.1],[-0.3,0.9],[-0.5,0.2]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 13 vertices and 17 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(Arpanet)
    </input>
  </session>

  <\center>
    <image|images/arpa.eps|35%|||>
  </center>

  Which edge should be added to the Arpanet to improve the reliability the
  most? Below is an analysis for the edge from Stanford to <abbr|CMU>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      R:=reliability_polynomial(Arpanet,p)
    <|unfolded-io>
      <\equation*>
        <around*|(|p-1|)><rsup|12>*<around|(|280*p<rsup|5>+310*p<rsup|4>+186*p<rsup|3>+63*p<rsup|2>+12*p+1|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=reliability_polynomial(add_edge(Arpanet,["STANFORD","CMU"]),p)
    <|unfolded-io>
      <\equation*>
        <around*|(|p-1|)><rsup|12>*<around|(|976*p<rsup|6>+1118*p<rsup|5>+703*p<rsup|4>+276*p<rsup|3>+72*p<rsup|2>+12*p+1|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      labels=["p","R,S"]; plot([R,S],p=0..1,color=[blue,red])
    </input>
  </session>

  <center|<image|images/plot.eps|35%|||>>

  The improvement is defined as the area enclosed by the above two curves,
  which can be computed as an integral.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      improvement:=integrate(S-R,p=0..1)
    <|unfolded-io>
      <\equation*>
        <frac|443879|10581480>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      evalf(improvement)
    <|unfolded-io>
      <\equation*>
        0.0419486688063
      </equation*>
    </unfolded-io>
  </session>

  \;

  <section|Connectivity>

  <subsection|Connected, biconnected and triconnected graphs>

  The commands <verbatim|is_connected><index|<verbatim|is_connected>>,
  <verbatim|is_biconnected><index|<verbatim|is_biconnected>> and
  <verbatim|is_triconnected><index|<verbatim|is_triconnected>> are used for
  determining if a graph is <hlink|connected|https://en.wikipedia.org/wiki/Connectivity_(graph_theory)#Connected_graph>,
  <hlink|biconnected|https://en.wikipedia.org/wiki/Biconnected_graph> or
  triconnected (<hlink|<rigid|3-connected>|https://en.wikipedia.org/wiki/K-vertex-connected_graph>),
  respectively.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_connected(G)>>>|<row|<cell|>|<cell|<verbatim|is_biconnected(G)>>>|<row|<cell|>|<cell|<verbatim|is_triconnected(G)>>>>>>

  Each of the above commands takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns <verbatim|true> if <math|G> possesses the
  required level of connectivity. Else, it returns <verbatim|false>.

  If <math|G> is directed, the edge directions are simply ignored (the
  commands operate on the underlying graph of <math|G>).

  The strategy for checking 1- and 2-connectivity is to use
  <hlink|depth-first search|https://en.wikipedia.org/wiki/Depth-first_search><nbsp>(see
  <cite-detail|gibbons|pp.<nbsp>20> and<nbsp><cite|tarjan72>). Both
  algorithms run in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>
  time. The algorithm for checking 3-connectivity is quite simple but less
  efficient: it works by choosing a vertex <math|v\<in\>V> and checking if
  the subgraph induced by <math|V\<setminus\><around*|{|v|}>> is biconnected,
  moving on to the next vertex if so, and repeating the process until all
  vertices are visited exactly once or a non-biconnected subgraph is found
  for some <math|v>. In the latter case the input graph is not triconnected.
  The complexity of this algorithm is <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_complement(complete_graph(2,3,4))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 9 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=connected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|0,1|]>,<around|[|2,3,4|]>,<around|[|5,6,7,8|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=induced_subgraph(G,C[2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(H)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(path_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(cycle_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_triconnected(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_triconnected(cycle_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Connected and biconnected components><label|connected-components>

  The command <verbatim|connected_components><index|<verbatim|connected_components>>
  resp.<nbsp><verbatim|biconnected_components><index|<verbatim|biconnected_components>>
  is used for decomposing a graph into <hlink|connected|https://en.wikipedia.org/wiki/Connected_component_(graph_theory)>
  resp.<nbsp><hlink|<rigid|biconnected components>|https://en.wikipedia.org/wiki/Biconnected_component>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|connected_components(G)>>>|<row|<cell|>|<cell|<verbatim|biconnected_components(G)>>>>>>

  <verbatim|connected_components> resp.<nbsp><verbatim|biconnected_components>
  takes a graph <math|G<around*|(|V,E|)>> as its only argument and returns
  the minimal partition <math|<around*|{|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>|}>>
  of <math|V> such that the subgraph <math|G<rsub|i>\<subset\>G> induced by
  <math|V<rsub|i>> is connected resp.<nbsp>biconnected for each
  <math|i=1,2,\<ldots\>,k>. The partition is returned as a list of lists
  <math|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>>.

  If <math|G> is directed, the edge directions are simply ignored (the
  commands operate on the underlying graph of <math|G>).

  The connected components of <math|G> are readily obtained by depth-first
  search in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.
  To find the biconnected components of <math|G>, <name|Tarjan>'s algorithm
  is used<nbsp><cite|tarjan72>, which also runs in linear time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_complement(complete_graph(3,5,7))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 34 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=connected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|0,1,2|]>,<around|[|3,4,5,6,7|]>,<around|[|8,9,10,11,12,13,14|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=highlight_subgraph(G,induced_subgraph(G,C[1]))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 34 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=highlight_subgraph(G,induced_subgraph(G,C[2]),magenta,cyan)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 34 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/ccomp.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=graph(trail(1,2,3,4,2),trail(4,5,6,7,5))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/bic.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(H)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      biconnected_components(H)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|2,3,4|]>,<around|[|4,5|]>,<around|[|5,6,7|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Vertex connectivity>

  The command <verbatim|vertex_connectivity><index|<verbatim|vertex_connectivity>>
  is used for computing <hlink|<rigid|vertex
  connectivity>|https://en.wikipedia.org/wiki/K-vertex-connected_graph> in
  undirected graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|vertex_connectivity(G)>>>>>>

  <verbatim|vertex_connectivity> takes an undirected connected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the largest
  integer <math|k> for which <math|G> is <math|k>-vertex-connected, meaning
  that <math|G> remains connected after removing fewer than <math|k> vertices
  from <math|V>.

  The strategy is to use the algorithm by <name|Esfahanian> and <name|Hakimi>
  <cite|esfahanian>, which is based on the maximum-flow computing approach by
  <name|Even> <cite-detail|even-algorithms|Section<nbsp>6.2>. The algorithm
  makes <math|<around*|\||V|\|>-\<delta\>-1+<frac|\<delta\>*<around*|(|\<delta\>-1|)>|2>>
  calls to <verbatim|<hlink|maxflow|#maxflow>> command, where
  <math|\<delta\>> is the minimum vertex degree in <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_connectivity(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_connectivity(graph("clebsch"))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(1000,0.5,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1000 vertices and 1876
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_connectivity(G)
    <|unfolded-io>
      <\equation*>
        2
      </equation*>

      <timing|3.28 sec>
    </unfolded-io>
  </session>

  <subsection|Graph rank>

  The command <verbatim|graph_rank><index|<verbatim|graph_rank>> is used for
  computing <hlink|<rigid|graph rank>|https://en.wikipedia.org/wiki/Rank_(graph_theory)>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_rank(G)>>>|<row|<cell|>|<cell|<verbatim|graph_rank(G,S)>>>>>>

  <verbatim|graph_rank> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally a set of edges <math|S\<subset\>E>
  (by default <math|S=E>), and returns <math|<around*|\||V|\|>-k> where
  <math|k> is the number of connected components of the spanning subgraph of
  <math|G> with edge set <math|S>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[3,4],[4,5]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_rank(G)
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_rank(G,[[1,2],[3,4]])
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Articulation points><label|articulation-points>

  The command <verbatim|articulation_points><index|<verbatim|articulation_points>>
  is used for obtaining the set of <hlink|articulation
  points|https://en.wikipedia.org/wiki/Biconnected_component> (cut-vertices)
  of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|articulation_points(G)>>>>>>

  <verbatim|articulation_points> takes a graph <math|G<around*|(|V,E|)>> as
  its only argument and returns the list of articulation points of <math|G>.
  A vertex <math|v\<in\>V> is an <strong|articulation point> of <math|G> if
  the removal of <math|v> increases the number of connected components of
  <math|G>.

  The articulation points of <math|G> are found by depth-first search in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>
  time<nbsp><cite|gibbons>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      articulation_points(path_graph([1,2,3,4]))
    <|unfolded-io>
      <\equation*>
        <around|[|2,3|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      length(articulation_points(cycle_graph(1,2,3,4)))
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Strongly connected components>

  The command <verbatim|strongly_connected_components><index|<verbatim|strongly_connected_components>>
  is used for decomposing digraphs into <hlink|<rigid|strongly connected
  components>|https://en.wikipedia.org/wiki/Strongly_connected_component>. A
  digraph <math|H> is <strong|strongly connected> if for each pair
  <math|<around*|(|v,w|)>> of distinct vertices in <math|H> there is a
  directed path from <math|v> to <math|w> in <math|H>. The command
  <verbatim|is_strongly_connected><index|<verbatim|is_strongly_connected>>
  can be used to determine whether a graph is strongly connected.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|strongly_connected_components(G)>>>|<row|<cell|>|<cell|<verbatim|is_strongly_connected(G)>>>>>>

  <verbatim|strongly_connected_components> takes a digraph
  <math|G<around*|(|V,E|)>> as its only argument and returns the minimal
  partition <math|<around*|{|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>|}>> of
  <math|V> such that the subgraph <math|G<rsub|i>\<subset\>G> induced by
  <math|V<rsub|i>> is strongly connected for each <math|i=1,2,\<ldots\>,k>.
  The result is returned as a list of lists
  <math|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>>.

  <verbatim|is_strongly_connected> takes a digraph <math|G> as its only
  argument and returns <verbatim|true> if <math|G> has exactly one strongly
  connected component and <verbatim|false> otherwise.

  The strategy is to use <name|Tarjan>'s algorithm for strongly connected
  components<nbsp><cite|tarjan72>, which runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph([1,2,3],%{[1,2],[1,3],[2,3],[3,2]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 3 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/digraph10.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_strongly_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      strongly_connected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1|]>,<around|[|2,3|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_digraph(10,18)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 10 vertices and 18 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <\center>
    <image|images/scomp.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      strongly_connected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|0,2,6|]>,<around|[|1|]>,<around|[|3|]>,<around|[|4|]>,<around|[|5,7,8|]>,<around|[|9|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Edge connectivity>

  The command <verbatim|edge_connectivity><index|<verbatim|edge_connectivity>>
  is used for computing the <hlink|<rigid|edge
  connectivity>|https://en.wikipedia.org/wiki/K-edge-connected_graph> of an
  undirected graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|edge_connectivity(G)>>>>>>

  <verbatim|edge_connectivity> takes an undirected connected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the largest
  integer <math|k> for which <math|G> is <math|k>-edge connected, meaning
  that <math|G> remains connected after fewer than <math|k> edges are removed
  from <math|E>.

  The strategy is to apply <name|Matula>'s algorithm
  <cite-detail|krishnaiyan|Section<nbsp>13.3.1>, which constructs a
  dominating set <math|D\<subset\>V> and calls
  <verbatim|<hlink|maxflow|#maxflow>> command <math|<around*|\||D|\|>-1>
  times.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=cycle_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edge_connectivity(G)
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edge_connectivity(K5)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edge_connectivity(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edge_connectivity(graph("clebsch"))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Edge cuts>

  The command <verbatim|is_cut_set><index|<verbatim|is_cut_set>> is used for
  determining whether a particular subset of edges of a graph is an
  <hlink|<rigid|edge cut>|https://en.wikipedia.org/wiki/Connectivity_(graph_theory)#Definitions_of_components,_cuts_and_connectivity>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_cut_set(G,L)>>>>>>

  <verbatim|is_cut_set> takes two arguments, a graph
  <math|G<around*|(|V,E|)>> and a subset <math|L\<subset\>E> of edges, and
  returns <verbatim|true> if the graph <math|G<rprime|'><around*|(|V,E\<setminus\>L|)>>
  has more connected components than <math|G>. Else it returns
  <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5,6,4,1,3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/cutset.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      E:=[[1,4],[3,4]]
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|4>>|<row|<cell|3>|<cell|4>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_cut_set(G,E)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(delete_edge(G,E))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Two-edge-connected graphs>

  The command <verbatim|is_two_edge_connected><index|<verbatim|is_two_edge_connected>>
  is used for determining whether an undirected graph is
  <hlink|<rigid|two-edge-connected>|https://en.wikipedia.org/wiki/K-edge-connected_graph>.
  The command <verbatim|two_edge_connected_components><index|<verbatim|two_edge_connected_components>>
  is used for splitting a graph into components having this property.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_two_edge_connected(G)>>>|<row|<cell|>|<cell|<verbatim|two_edge_connected_components(G)>>>>>>

  <verbatim|is_two_edge_connected> takes an undirected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns <verbatim|true>
  if <math|G> has no bridges, i.e.<nbsp>edges which removal increases the
  number of connected components of <math|G>.

  <verbatim|two_edge_connected_components> takes an undirected graph
  <math|G<around*|(|V,E|)>> and returns the list of two-edge-connected
  components of <math|G>, each of them represented by the list of its
  vertices. To obtain a component as a graph, use the
  <verbatim|<hlink|induced_subgraph|#induced-subgraph>> command.

  The strategy for finding bridges<nbsp><cite|tarjan-bridges> is similar to
  finding <hlink|<rigid|articulation points>|#articulation-points>. Once the
  bridges of <math|G> are found, it is easy to split <math|G> into
  two-edge-connected components by removing the bridges and returning the
  list of <hlink|<rigid|connected components>|#connected-components> of the
  resulting graph. Both algorithms run in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_two_edge_connected(cycle_graph(4))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_two_edge_connected(path_graph(4))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{["a","b"],["b","c"],["a","c"],["d","e"],["e","f"],["d","f"],["c","d"],["a","h"],["a","i"],["h","i"]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_two_edge_connected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/tec.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C:=two_edge_connected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|a,b,c,h,i|]>,<around|[|d,e,f|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  To visualize the bridges of <math|G>, one can highlight the edges of each
  component. The remaining (unhighlighted) edges are the bridges.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      for c in C do G:=highlight_edges(G,edges(induced_subgraph(G,c))); od:;
    </input>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/tec2.eps|35%|||>>

  <section|Trees>

  <subsection|Tree graphs><label|is-tree>

  The command <verbatim|is_tree><index|<verbatim|is_tree>> is used for
  determining whether a graph is a <hlink|tree|https://en.wikipedia.org/wiki/Tree_(graph_theory)>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_tree(G)>>>>>>

  <verbatim|is_tree> takes a graph <math|G<around*|(|V,E|)>> as its only
  argument and returns <verbatim|true> if <math|G> is undirected, connected
  and <math|<around*|\||V|\|>=<around*|\||E|\|>+1>. Else it returns
  <verbatim|false>.

  The only expensive step in the algorithm is determining whether <math|G> is
  connected. The condition <math|<around*|\||V|\|>=<around*|\||E|\|>+1> is
  checked first, hence the algorithm runs in
  <math|O<around*|(|<around*|\||V|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tree(complete_binary_tree(3))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tree(cycle_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Forest graphs>

  The command <verbatim|is_forest><index|<verbatim|is_forest>> is used for
  determining whether a graph is a <hlink|forest|https://en.wikipedia.org/wiki/Tree_(graph_theory)#Forest>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_forest(G)>>>>>>

  <verbatim|is_forest> takes the a <math|G<around*|(|V,E|)>> as its only
  argument and returns <verbatim|true> if every connected component of
  <math|G> is a tree and <verbatim|false> otherwise.

  The algorithm runs in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>
  time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      F:=disjoint_union(apply(random_tree,[k$(k=10..30)]))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 420 vertices and 399 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_connected(F)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_forest(F)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(F)
    </input>
  </session>

  <center|<image|images/forest.eps|35%|||>>

  <subsection|Height of a tree>

  The command <verbatim|tree_height><index|<verbatim|tree_height>> is used
  for determining the height of a tree with respect to the specified root
  node. The <strong|height> of a tree <math|T> is the length of the longest
  path in <math|T> that has the root node of <math|T >as one of its
  endpoints.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|tree_height(G,r)>>>>>>

  <verbatim|tree_height> takes two arguments, a tree graph
  <math|G<around*|(|V,E|)>> and a vertex <math|r\<in\>V>, which is used as
  the root node. The command returns the height of <math|G> with respect to
  <math|r>.

  The strategy is to start a depth-first search from the root node and look
  for the deepest node. Therefore the algorithm runs in
  <math|O<around*|(|<around*|\||V|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tree(1000)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1000 vertices and 999
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      r:=rand(1000)
    <|unfolded-io>
      <\equation*>
        296
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      tree_height(G,r)
    <|unfolded-io>
      <\equation*>
        20
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Lowest common ancestor of a pair of nodes>

  The command <verbatim|lowest_common_ancestor><index|<verbatim|lowest_common_ancestor>>
  is used for computing the <hlink|<rigid|lowest common
  ancestor>|https://en.wikipedia.org/wiki/Lowest_common_ancestor>
  (<abbr|LCA>) of a pair of nodes in a tree or for each element of a list of
  such pairs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|lowest_common_ancestor(G,r,u,v)>>>|<row|<cell|>|<cell|<verbatim|lowest_common_ancestor(G,r,[[u1,v1],[u2,v2],..,[uk,vk]])>>>>>>

  <verbatim|lowest_common_ancestor> takes two mandatory arguments, a tree
  graph <math|G<around*|(|V,E|)>> and the root node <math|r\<in\>V>. There
  are two possibilities for specifying the nodes to operate on: either the
  nodes <math|u,v\<in\>V> are given as the third and the fourth argument, or
  a list of pairs of nodes <math|<around*|(|u<rsub|1>,v<rsub|1>|)>,<around*|(|u<rsub|2>,v<rsub|2>|)>,\<ldots\>,<around*|(|u<rsub|k>,v<rsub|k>|)>>,
  where <math|u<rsub|i>,v<rsub|i>\<in\>V> and
  <math|u<rsub|i>\<neq\>v<rsub|i>> for <math|i=1,2,\<ldots\>,k>, is given as
  the third argument. The command returns the <abbr|LCA> of <math|u> and
  <math|v> or the list containing <abbr|LCA> of every pair of nodes
  <math|u<rsub|i>,v<rsub|i>> for <math|i=1,2,\<ldots\>,k>. Note that this is
  much faster than calling <verbatim|lowest_common_ancestor> <math|k> times
  with a single pair of vertices each time.

  The strategy is to use <name|Tarjan>'s offline <abbr|LCA>
  algorithm<nbsp><cite|lca>, which runs in nearly linear time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tree(25)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 25 vertices and 24 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <\center>
    <image|images/lca.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      lowest_common_ancestor(G,0,19,22)
    <|unfolded-io>
      <\equation*>
        16
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      lowest_common_ancestor(G,0,[[5,13],[17,24],[9,16]])
    <|unfolded-io>
      <\equation*>
        <around|[|4,16,0|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Arborescence graphs>

  The command <verbatim|is_arborescence><index|<verbatim|is_arborescence>> is
  used for determining whether a directed unweighted graph is an
  <hlink|arborescence|https://en.wikipedia.org/wiki/Arborescence_(graph_theory)>
  (which is the digraph form of a rotted tree).

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_arborescence(G)>>>>>>

  <verbatim|is_arborescence> takes a digraph <math|G<around*|(|V,E|)>> as its
  only argument and returns <verbatim|true> if there is a vertex
  <math|u\<in\>V> such that for any other <math|v\<in\>V> there is exactly
  one directed path from <math|u> to <math|v>. Else it returns
  <verbatim|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=digraph(%{[1,2],[1,3],[3,4],[3,5],[3,6],[5,7]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 7 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_arborescence(T)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T)
    </input>
  </session>

  <center|<image|images/arbor.eps|35%|||>>

  <section|Networks>

  <subsection|Network graphs><label|is-network>

  The command <verbatim|is_network><index|<verbatim|is_network>> is used for
  determining whether a graph is a <hlink|<rigid|flow
  network>|http://www.encyclopediaofmath.org/index.php?title=Flow_in_a_network>.
  In this context, a flow network is directed, connected graph with at least
  one vertex with in-degree 0 (the <strong|source>) and at least one vertex
  with out-degree 0 (the <strong|sink>).

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_network(G)>>>|<row|<cell|>|<cell|<verbatim|is_network(G,s,t)>>>>>>

  <verbatim|is_network> takes one or three arguments, a digraph
  <math|G<around*|(|V,E|)>> and optionally the source vertex <math|s> and the
  sink vertex <math|t>. If these vertices are given, the command returns
  <verbatim|true> if <math|G> is a network with respect to <math|s>, <math|t>
  and <verbatim|false> otherwise. If the graph <math|G> is given as the only
  argument, the command returns a sequence of two objects, the list of all
  sources in <math|G> and the list of all sinks in <math|G>, respectively. If
  one of these lists is empty, then <math|G> is implicitly not a network
  (both lists are empty if <math|G> is not connected).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N:=digraph(%{[1,2],[1,3],[2,4],[3,4]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N,spring)
    </input>
  </session>

  <center|<image|images/network3.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N,1,4)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N,2,3)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(%{[1,3],[2,3],[3,4],[3,5]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle)
    </input>
  </session>

  <center|<image|images/network4.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|2>>|<row|<cell|4>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Maximum flow><label|maxflow>

  The command <verbatim|maxflow><index|<verbatim|maxflow>> is used for
  computing the <hlink|<rigid|maximum flow>|https://en.wikipedia.org/wiki/Maximum_flow_problem>
  in a network.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|maxflow(G,s,t)>>>|<row|<cell|>|<cell|<verbatim|maxflow(G,s,t,M)>>>>>>

  <verbatim|maxflow> takes three or four arguments: a network graph
  <math|G<around*|(|V,E|)>>, the source <math|s\<in\>V>, the sink
  <math|t\<in\>V> and optionally an unassigned identifier <verbatim|M>. It
  returns the optimal value for the maximum flow problem for the network
  <math|<around*|(|G,s,t|)>>. If the fourth argument is given, an optimal
  flow is written to <verbatim|M> in form of a matrix.

  The strategy is to use the algorithm of <name|Edmonds> and <name|Karp>
  <cite|edmonds-karp>, which solves the maximum flow problem in
  <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|><rsup|2>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=[[0,1,0,4,0,0],[0,0,1,0,3,0],[0,1,0$3,1],[0,0,3,0,1,0],[0$3,1,0,4],[0$6]]
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|6|6|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|0>|<cell|4>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|3>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|3>|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|1>|<cell|0>|<cell|4>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N:=digraph([1,2,3,4,5,6],A)
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 6 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|1|1|cell-rborder|0ln>|<table|<row|<cell|1>>|<row|<cell|6>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N,spring)
    </input>
  </session>

  <center|<image|images/network5.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maxflow(N,1,6,M)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|6|6|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|0>|<cell|3>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|2>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>|<cell|0>|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|0>|<cell|2>|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|3>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      N:=random_network(2,3,0.9,acyclic,weights=5..15)
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 12 vertices and 19 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_network(N)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|1|1|cell-rborder|0ln>|<table|<row|<cell|0>>|<row|<cell|11>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(N,spring)
    </input>
  </session>

  <\center>
    <image|images/network6.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maxflow(N,0,11,F)
    <|unfolded-io>
      <\equation*>
        17
      </equation*>
    </unfolded-io>
  </session>

  To visualize the optimal flow <math|F>, one can use the
  <verbatim|<hlink|highlight_subgraph|#highlight-subgraph>> command with the
  option <verbatim|weights> to display the actual flow in the highlighted
  edges. Non-highlighted edges have zero flow.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(N,digraph(vertices(N),F),weights),spring)
    </input>
  </session>

  <\center>
    <image|images/network7.eps|35%|||>
  </center>

  <subsection|Minimum cut>

  The command <verbatim|minimum_cut><index|<verbatim|minimum_cut>> is used
  for obtaining <hlink|<rigid|minimum cuts>|https://en.wikipedia.org/wiki/Minimum_cut>
  in networks.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|minimum_cut(G,s,t)>>>>>>

  <verbatim|minimum_cut> takes three arguments, a digraph
  <math|G<around*|(|V,E|)>> and two vertices <math|s,t\<in\>V> such that
  <around*|(|<math|G,s,t>|)> is a network with source <math|s> and sink
  <math|t>. The returned value is a list of edges in <math|E> representing a
  minimum cut in the network.

  The strategy is to apply the command <verbatim|maxflow>, which finds a
  maximal flow, and to run depth-first search on the corresponding residual
  graph to find a <math|S,T> partition of <math|V>. The minimum cut is then
  the set of all arcs <math|v w\<in\>E> such that <math|v\<in\>S> and
  <math|w\<in\>T>. The algorithm runs in <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|><rsup|2>|)>>
  time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(%{[[0,1],16],[[0,2],13],[[1,2],10],[[1,3],12],[[2,1],4],[[2,4],14],[[3,2],9],[[3,5],20],[[4,3],7],[[4,5],4]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 6 vertices and 10 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/mincut.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cut:=minimum_cut(G,0,5)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|3>>|<row|<cell|4>|<cell|3>>|<row|<cell|4>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,cut),spring)
    </input>
  </session>

  <center|<draw-over|<image|images/mincut2.eps|35%|||>|<with|gr-mode|<tuple|edit|spline>|gr-dash-style|11100|<graphics|<with|dash-style|11100|<spline|<point|0.117277417647837|1.12349517131896>|<point|0.251819023680381|0.420889006482339>|<point|2.94265114433126|-0.341513427702077>|<point|2.82305860563567|-1.07401772721259>>>>>|0cm>>

  By the <hlink|<rigid|max-flow min-cut theorem>|https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem>,
  the sum of edge weights in minimum cut is equal to the value of maximum
  flow.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      w:=0:; for ed in cut do w:=w+get_edge_weight(G,ed); od:; w
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>,23
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maxflow(G,0,5)
    <|unfolded-io>
      <\equation*>
        23
      </equation*>
    </unfolded-io>
  </session>

  <section|Distance in graphs>

  <subsection|Vertex distance><label|vertex-distance>

  The command <verbatim|vertex_distance><index|<verbatim|vertex_distance>> is
  used for computing the length of the shortest path(s) from the source
  vertex to some other vertex/vertices of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|vertex_distance(G,v,w)>>>|<row|<cell|>|<cell|<verbatim|vertex_distance(G,v,L)>>>>>>

  <verbatim|vertex_distance> takes three arguments, a graph
  <math|G<around*|(|V,E|)>>, a vertex <math|v\<in\>V> called the
  <strong|source> and a vertex <math|w\<in\>V> called the <strong|target> or
  a list <math|L\<subset\>V\<setminus\><around*|{|v|}>> of target vertices.
  The command returns the distance between <math|v> and <math|w> as the
  number of edges in a shortest path from <math|v> to <math|w>, or the list
  of distances if a list of target vertices is given.

  The strategy is to use <hlink|breadth-first
  search|https://en.wikipedia.org/wiki/Breadth-first_search>
  <cite-detail|gibbons|pp.<nbsp>35> starting from the source vertex.
  Therefore, the algorithm runs in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>
  time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_distance(G,1,3)
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertex_distance(G,1,[3,6,9])
    <|unfolded-io>
      <\equation*>
        <around|[|2,1,2|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|All-pairs vertex distance><label|allpairs-distance>

  The command <verbatim|allpairs_distance><index|<verbatim|allpairs_distance>>
  is used for computing the matrix of distances between all pairs of vertices
  in a (weighted) graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|allpairs_distance(G)>>>>>>

  <verbatim|allpairs_distance> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns a square matrix <math|D=<around*|[|d<rsub|i
  j>|]>> with <math|n=<around*|\||V|\|>> rows and columns such that
  <math|d<rsub|i j>=distance<around*|(|v<rsub|i>,v<rsub|j>|)>> for all
  <math|i,j=1,2,\<ldots\>,n>, where <math|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>>
  are the elements of <math|V>. If <math|v<rsub|i> v<rsub|j>\<nin\>E>, then
  <math|d<rsub|i j>=+\<infty\>>. The strategy is to apply the algorithm of
  <name|Floyd> and <name|Warshall> <cite|floyd>, which runs in
  <math|O<around*|(|<around*|\||V|\|><rsup|3>|)>> time.

  Note that, if <math|G> is weighted, it must not contain negative cycles. A
  cycle is <strong|negative> if the sum of weights of its edges is negative.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph([1,2,3,4,5],%{[1,2],[1,3],[1,4],[1,5],[2,3],[3,4],[4,5],[5,2]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      allpairs_distance(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|5|5|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|1>|<cell|1>|<cell|1>>|<row|<cell|1>|<cell|0>|<cell|1>|<cell|2>|<cell|1>>|<row|<cell|1>|<cell|1>|<cell|0>|<cell|1>|<cell|2>>|<row|<cell|1>|<cell|2>|<cell|1>|<cell|0>|<cell|1>>|<row|<cell|1>|<cell|1>|<cell|2>|<cell|1>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=digraph(%{[1,2],[1,3],[1,4],[1,5],[2,3],[3,4],[4,5],[5,2]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 8 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      allpairs_distance(H)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|5|5|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>|<cell|1>|<cell|1>|<cell|1>>|<row|<cell|+\<infty\>>|<cell|0>|<cell|1>|<cell|2>|<cell|3>>|<row|<cell|+\<infty\>>|<cell|3>|<cell|0>|<cell|1>|<cell|2>>|<row|<cell|+\<infty\>>|<cell|2>|<cell|3>|<cell|0>|<cell|1>>|<row|<cell|+\<infty\>>|<cell|1>|<cell|2>|<cell|3>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/floyd.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=assign_edge_weights(H,5,25)
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 5 vertices and 8 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/floydw.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      allpairs_distance(H)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|5|5|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|24>|<cell|18>|<cell|18>|<cell|8>>|<row|<cell|+\<infty\>>|<cell|0>|<cell|14>|<cell|25>|<cell|30>>|<row|<cell|+\<infty\>>|<cell|41>|<cell|0>|<cell|11>|<cell|16>>|<row|<cell|+\<infty\>>|<cell|30>|<cell|44>|<cell|0>|<cell|5>>|<row|<cell|+\<infty\>>|<cell|25>|<cell|39>|<cell|50>|<cell|0>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Diameter>

  The command <verbatim|graph_diameter> is used for determining the maximum
  distance among all pairs of vertices in a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|graph_diameter(G)>>>>>>

  <verbatim|graph_diameter> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns the number <math|max<around*|{|distance<around*|(|u,v|)>\<of\>u,v\<in\>V|}>>.
  If <math|G> is disconnected, <math|+\<infty\>> is returned.

  This command calls <verbatim|<hlink|allpairs_distance|#allpairs-distance>>
  and picks the largest element in the output matrix. Hence the complexity of
  the algorithm is <math|O<around*|(|<around*|\||V|\|><rsup|3>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_diameter(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_diameter(cycle_graph(19))
    <|unfolded-io>
      <\equation*>
        9
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_diameter(disjoint_union(graph("petersen"),cycle_graph(19)))
    <|unfolded-io>
      <\equation*>
        +\<infty\>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[[1,2],0.2],[[2,3],0.3],[[3,4],0.4],[[4,1],1.1]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/diam.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph_diameter(G)
    <|unfolded-io>
      <\equation*>
        0.9
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      dijkstra(G,1,4)
    <|unfolded-io>
      <\equation*>
        <around|[|<around|[|1,2,3,4|]>,0.9|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Girth>

  The commands <verbatim|girth><index|<verbatim|girth>> and
  <verbatim|odd_girth><index|<verbatim|odd_girth>> are used for computing the
  (odd) girth of an undirected unweighted graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|girth(G)>>>>>>

  <verbatim|girth> resp.<nbsp><verbatim|odd_girth> takes a graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the girth
  resp.<nbsp>odd girth of <math|G>. The (odd) girth of <math|G> is defined to
  be the length of the shortest (odd) cycle in <math|G>. If there is no (odd)
  cycle in <math|G>, the command returns <math|+\<infty\>>.

  The strategy is to apply breadth-first search from each vertex of the input
  graph. The runtime is therefore <math|O<around*|(|<around*|\||V|\|>*\|*E<around*|\|||\<nobracket\>>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      girth(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=hypercube_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 8 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=subdivide_edges(G,["000","001"])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 9 vertices and 13 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      girth(G)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      odd_girth(G)
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      girth(complete_binary_tree(2))
    <|unfolded-io>
      <\equation*>
        +\<infty\>
      </equation*>
    </unfolded-io>
  </session>

  <section|Acyclic graphs>

  <subsection|Acyclic graphs>

  The command <verbatim|is_acyclic><index|<verbatim|is_acyclic>> is used for
  checking for absence of directed cycles in digraphs. A directed graph with
  no directed cycle is said to be <strong|acyclic>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_acyclic(G)>>>>>>

  <verbatim|is_acyclic> takes a digraph <math|G<around*|(|V,E|)>> as its only
  argument and returns <verbatim|true> if <math|G> is acyclic and
  <verbatim|false> otherwise.

  The algorithm attempts to find topological order for its vertices. If that
  succeeds, the graph is acyclic, otherwise not. The algorithm runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_acyclic(digraph(trail(1,2,3,4,5)))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_acyclic(digraph(trail(1,2,3,4,5,2)))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Topological sorting>

  The command <verbatim|topologic_sort><index|<verbatim|topologic_sort>> or
  <verbatim|topological_sort><index|<verbatim|topological_sort>> is used for
  finding a linear ordering of vertices of an acyclic digraph which is
  consistent with the arcs of the digraph. This procedure is called
  <hlink|<rigid|topological sorting>|https://en.wikipedia.org/wiki/Topological_sorting>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|topologic_sort(G)>>>|<row|<cell|>|<cell|<verbatim|topological_sort(G)>
  (alias)>>>>>

  <verbatim|topologic_sort> takes a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns the list of vertices of <math|G> in a particular
  order: a vertex <math|u> precedes a vertex <math|v> if <math|u*v\<in\>E>,
  i.e.<nbsp>if there is an arc from <math|u> to <math|v>.

  Note that topological sorting is possible only if the input graph is
  acyclic. If this condition is not met, <verbatim|topologic_sort> returns an
  error. Otherwise, it finds the required ordering by applying <name|Kahn>'s
  algorithm<nbsp><cite|kahn>, which runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph(%{[c,a],[c,b],[c,d],[a,d],[b,d],[a,b]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_acyclic(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      topologic_sort(G)
    <|unfolded-io>
      <\equation*>
        <around|[|c,a,b,d|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|<samp|st> ordering><label|st-ordering>

  The command <verbatim|st_ordering><index|<verbatim|st_ordering>> is used
  for finding <hlink|<samp|st>-orderings|https://en.wikipedia.org/wiki/Bipolar_orientation>
  in undirected biconnected graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|st_ordering(G,s,t,[p])>>>|<row|<cell|>|<cell|<verbatim|st_ordering(G,s,t,D,[p])>>>>>>

  <verbatim|st_ordering> takes three to five arguments. The first three
  arguments are mandatory: an undirected biconnected graph
  <math|G<around*|(|V,E|)>>, a vertex <math|s\<in\>V> called the source, a
  vertex <math|t\<in\>V> called the sink such that <math|s t\<in\>E>.
  Optionally, one can pass an unassigned identifier <verbatim|D> and/or a
  real value <math|p\<in\><around*|[|0,1|]>>. The command returns the
  permutation <math|\<sigma\>> of the vertex set <math|V> which corresponds
  to <samp|st>-numbering of the vertices. Now, an orientation of each
  <math|e=u v\<in\>E> can be determined by the ordinals <math|n> and <math|m>
  of its endpoints <math|u> and <math|v>, respectively, which are assigned by
  the permutation <math|\<sigma\>>: if <math|n\<less\>m>, then <math|u> is
  the head and <math|v> is the tail of the corresponding arc, and vice versa
  otherwise. If an identifier <verbatim|D> is given, a copy of <math|G>,
  which is made directed according to these orientations, is stored to
  <verbatim|D>. The oriented variant of <math|G> is an acyclic graph (or
  <abbr|DAG> for short).

  The requirement that the input graph is biconnected implies that
  <samp|st>-ordering can be computed for any pair <math|s,t\<in\>V> such that
  <math|s t\<in\>E>.

  If <math|p> is not specified, the strategy is to apply <name|Tarjan>'s
  algorithm<nbsp><cite|tarjan86> which runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time. When
  <math|p\<in\><around*|[|0,1|]>> is given, a parametrized <samp|st>-ordering
  is computed, in which the length of the longest path from <math|s> to
  <math|t> in the respective <abbr|DAG> roughly corresponds to
  <math|p*<around*|\||V|\|>>. Thus by varying <math|p> one controls the
  length of the longest directed path from <math|s> to <math|t>. The
  parametrized branch of the algorithm is implemented according to
  <name|Papamanthou><nbsp><cite|cpap> and runs in
  <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[a,b],[a,c],[a,d],[b,c],[b,d],[c,d]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      vertices(G)
    <|unfolded-io>
      <\equation*>
        <around|[|a,b,c,d|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      st_ordering(G,a,d,D)
    <|unfolded-io>
      <\equation*>
        <around|[|0,2,1,3|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(D)
    </input>
  </session>

  <center|<image|images/digraph11.eps|35%|||>>

  The following program demonstrates using the parametrized
  <samp|st>-ordering algorithm in order to find a path between vertices
  <math|u> and <math|v> in an undirected, biconnected graph
  <math|G<around*|(|V,E|)>>. The path length is controllable by varying the
  parameter <math|p\<in\><around*|[|0,1|]>>.

  <\verbatim-code>
    FindPath:=proc(G,u,v,p)

    \ \ local tmp,D,W;

    \ \ tmp:=!has_edge(G,[u,v]);

    \ \ if tmp then G:=add_edge(G,[u,v]); fi;

    \ \ purge(D);

    \ \ st_ordering(G,u,v,D,p);

    \ \ if tmp then D:=delete_arc(D,[u,v]); fi;

    \ \ W:=is_weighted(G)?weight_matrix(G):adjacency_matrix(G);

    \ \ D:=make_weighted(D,-W);

    \ \ return bellman_ford(D,u,v)[0];

    end:;
  </verbatim-code>

  The procedure <verbatim|FindPath> uses the <hlink|Bellman-Ford
  algorithm|#cheapest-paths> to find a longest path from the vertex
  <math|u\<in\>V> to the vertex <math|v\<in\>V> in the <abbr|DAG> <math|D>
  induced by a parametrized <samp|st>-ordering of <math|G> with parameter
  <math|p>. To trick Bellman-Ford into finding a longest path instead of the
  shortest one (which it was designed for), the edges of <math|D> are
  weighted with negative weights. Since <math|D> is acyclic, it contains no
  negative cycles, so the Bellman-Ford algorithm terminates successfully.

  For <math|p=0> one obtains a relatively short path, but usually not a
  minimal one. For <math|p=1> one obtains near-Hamiltonian paths. For
  <math|p\<in\><around*|(|0,1|)>> one obtains a path of length <math|l> which
  obeys the relation

  <\equation*>
    l\<approx\>l<rsub|0>+p*<around*|(|<around*|\||V|\|>-l<rsub|0>|)>,
  </equation*>

  where <math|l<rsub|0>> is the average path length for <math|p=0>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("soccerball")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 90 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P1:=FindPath(G,3,33,0):; length(P1)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,12
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P2:=FindPath(G,3,33,0.5):; length(P2)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,39
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P3:=FindPath(G,3,33,1):; length(P3)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,59
      </equation*>
    </unfolded-io>
  </session>

  <section|Matching in graphs>

  <subsection|Maximum matching><label|maximum-matching>

  The command <verbatim|maximum_matching><index|<verbatim|maximum_matching>>
  is used for finding maximum <hlink|matchings|https://en.wikipedia.org/wiki/Matching_(graph_theory)>
  <cite-detail|godsil|pp.<nbsp>43> in undirected unweighted graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|maximum_matching(G)>>>>>>

  <verbatim|maximum_matching> takes an undirected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns a list of edges
  <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|m>\<in\>E> such that
  <math|e<rsub|i>> and <math|e<rsub|j>> are not adjacent (i.e.<nbsp>have no
  common endpoints) for all <math|1\<leqslant\>i\<less\>j\<leqslant\>m> and
  that <math|m> is maximal. The return value can be interpreted as the list
  of matched pairs of vertices in <math|G>.

  The strategy is to apply the <hlink|<rigid|blossom
  algorithm>|https://en.wikipedia.org/wiki/Blossom_algorithm> of
  <name|Edmonds> <cite|edmonds>, which runs in
  <math|O<around*|(|<around*|\||V|\|><rsup|2>*<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maximum_matching(graph("octahedron"))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|6>>|<row|<cell|3>|<cell|2>>|<row|<cell|5>|<cell|4>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("soccerball")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 90 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=maximum_matching(G):; length(M)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,30
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,M),labels=false)
    </input>
  </session>

  <\center>
    <image|images/matching1.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(1000,10,5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1000 vertices and 13993
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      length(maximum_matching(G))
    <|unfolded-io>
      <\equation*>
        500
      </equation*>

      <timing|181 sec>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("blanusa")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 18 vertices and 27 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,maximum_matching(G)),labels=false)
    </input>
  </session>

  <center|<image|images/matching3.eps|35%|||>>

  \;

  <subsection|Maximum matching in bipartite graphs>

  The command <verbatim|bipartite_matching><index|<verbatim|bipartite_matching>>
  is used for finding maximum matchings in undirected, unweighted bipartite
  graphs. It applies the algorithm of <name|Hopcroft> and
  <name|Karp><nbsp><cite|hopcroft>, which is more efficient than the general
  algorithm used by the command <verbatim|<hlink|maximum_matching|#maximum-matching>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|bipartite_matching(G)>>>>>>

  <verbatim|bipartite_matching> takes an undirected bipartite graph
  <math|G<around*|(|V,E|)>> as its only argument and returns a sequence
  containing two elements: the size of the matching and the list of edges
  connecting matched pairs of vertices. The algorithm runs in
  <math|O<around*|(|<sqrt|<around*|\||V|\|>>*<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("desargues")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_bipartite(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=bipartite_matching(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|2>|<cell|3>>|<row|<cell|4>|<cell|5>>|<row|<cell|6>|<cell|7>>|<row|<cell|8>|<cell|9>>|<row|<cell|10>|<cell|13>>|<row|<cell|11>|<cell|18>>|<row|<cell|12>|<cell|15>>|<row|<cell|14>|<cell|17>>|<row|<cell|16>|<cell|19>>>>>|)>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,M))
    </input>
  </session>

  <center|<image|images/matching2.eps|35%|||>>

  <section|Cliques>

  <subsection|Clique graphs>

  To check whether an undirected graph is complete, one can use the
  <kbd|is_clique><index|<kbd|is_clique>> command.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_clique(G)>>>>>>

  <kbd|is_clique> takes an undirected graph <math|G<around*|(|V,E|)>> as its
  only argument and returns <kbd|true> if every pair of distinct vertices is
  connected by a unique edge in <math|E>. Else, it returns <kbd|false>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      K5:=complete_graph(5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 10 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_clique(K5)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=delete_edge(K5,[1,2])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_clique(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Maximal cliques>

  Given an undirected graph <math|G<around*|(|V,E|)>>, a subset
  <math|S\<subset\>V> is called a <strong|clique> in <math|G> if any two
  distinct vertices from <math|S> are adjacent in <math|G>, i.e.<nbsp>if the
  subgraph of <math|G> induced by the set <math|S> is complete. A clique is
  <strong|maximal> if it cannot be extended by adding more vertices from
  <math|V> to it. To count all maximal cliques in a graph (and optionally
  list them) one can use the <kbd|clique_stats><index|<kbd|clique_stats>>
  command.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|clique_stats(G,[C])>>>|<row|<cell|>|<cell|<verbatim|clique_stats(G,k,[C])>>>|<row|<cell|>|<cell|<verbatim|clique_stats(G,m..n,[C])>>>>>>

  <kbd|clique_stats> takes an undirected graph <math|G<around*|(|V,E|)>> as
  the mandatory first argument. If no other arguments are given, the command
  returns a list of pairs, each pair consisting of two integers: clique
  cardinality <math|k> and the number <math|n<rsub|k>\<gtr\>0> of
  <math|k>-cliques in <math|G>, respectively. (Therefore, the sum of second
  members of all returned pairs is equal to the total count of all maximal
  cliques in <math|G>.) If two arguments are passed to
  <verbatim|clique_stats>, the second argument must be a positive integer
  <math|k> or an interval with integer bounds <math|m><nbsp>..<nbsp><math|n>.
  In the first case the number of <math|k>-cliques is returned; in the second
  case, only cliques with cardinality between <math|m> and <math|n>
  (inclusive) are counted.

  If <verbatim|C> is specified as the last argument, it must be an unassigned
  identifier. Maximal cliques are in that case stored to <verbatim|C> as a
  list of lists of cliques of equal size. This option is therefore used for
  listing all maximal cliques.

  The strategy used to find all maximal cliques is a variant of the algorithm
  of <name|Bron> and <name|Kerbosch> developed by <name|Tomita> et
  al.<nbsp><cite|tomita>. Its worst-case running time is
  <math|O<around*|(|3<rsup|<around*|\||V|\|>/3>|)>>. However, the algorithm
  is usually very fast, typically taking only a moment for graphs with few
  hundred vertices or less.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(50,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 50 vertices and 633 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|3>|<cell|2>>|<row|<cell|4>|<cell|123>>|<row|<cell|5>|<cell|465>>|<row|<cell|6>|<cell|388>>|<row|<cell|7>|<cell|73>>|<row|<cell|8>|<cell|6>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(100,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 2448
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G,5)
    <|unfolded-io>
      <\equation*>
        4080
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(500,0.25)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 500 vertices and 31400
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G,5..7)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|5>|<cell|158436>>|<row|<cell|6>|<cell|19507>>|<row|<cell|7>|<cell|383>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G,C)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|3>|<cell|8>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|3>|<cell|6>>|<row|<cell|1>|<cell|3>|<cell|5>>|<row|<cell|1>|<cell|6>|<cell|4>>|<row|<cell|1>|<cell|5>|<cell|4>>|<row|<cell|3>|<cell|6>|<cell|2>>|<row|<cell|3>|<cell|5>|<cell|2>>|<row|<cell|6>|<cell|4>|<cell|2>>|<row|<cell|5>|<cell|4>|<cell|2>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Maximum clique>

  Any largest maximal clique in an undirected graph is called <strong|maximum
  clique>. The command <rigid|<kbd|maximum_clique>><index|<kbd|maximum_clique>>
  can be used to find one in a graph. If only the size of a maximum clique is
  desired, one can use the command <verbatim|clique_number><index|<verbatim|clique_number>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|maximum_clique(G)>>>|<row|<cell|>|<cell|<verbatim|clique_number(G)>>>>>>

  <kbd|maximum_clique> takes an undirected graph <math|G> as its only
  argument and returns a maximum clique in <math|G> as a list of vertices.
  The clique may subsequently be extracted from <math|G> using the command
  <verbatim|<hlink|induced_subgraph|#induced-subgraph>>.

  The strategy used to find maximum clique is an improved variant of the
  classical algorithm by <name|Carraghan> and <name|Pardalos> developed by
  <name|Östergård> <cite|ostergard>.

  In the following examples, the results were obtained almost instantly.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=sierpinski_graph(5,5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3125 vertices and 7810
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maximum_clique(G)
    <|unfolded-io>
      <\equation*>
        <around|[|1560,1561,1562,1563,1564|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(300,0.3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 300 vertices and 13352
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maximum_clique(G)
    <|unfolded-io>
      <\equation*>
        <around|[|60,80,111,201,248,252,288|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_complement(complete_graph(4,3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cliq:=maximum_clique(G)
    <|unfolded-io>
      <\equation*>
        <around|[|0,1,2,3|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(G,induced_subgraph(G,cliq)))
    </input>
  </session>

  <center|<image|images/cliq.eps|35%|||>>

  <verbatim|clique_number> takes an undirected graph <math|G> as its only
  argument and returns the number of vertices forming a maximum clique in
  <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_number(G)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Minimum clique cover><label|minimum-covering>

  A <strong|minimum clique cover> for an undirected graph <math|G> is any
  minimal set <math|S=<around*|{|C<rsub|1>,C<rsub|2>,\<ldots\>,C<rsub|k>|}>>
  of cliques in <math|G> such that for every vertex <math|v> in <math|G>
  there exists <math|i\<leqslant\>k> such that <math|v\<in\>C<rsub|i>>. Such
  a cover can be obtained by calling the <kbd|clique_cover><index|<kbd|clique_cover>>
  command.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|clique_cover(G)>>>|<row|<cell|>|<cell|<verbatim|clique_cover(G,k)>>>>>>

  <kbd|clique_cover> takes an undirected graph <math|G<around*|(|V,E|)>> as
  its mandatory argument and returns the smallest possible cover. Optionally,
  a positive integer <math|k> may be passed as the second argument. In that
  case the requirement that <math|k> is less or equal to the given integer is
  set. If no such cover is found, <kbd|clique_cover> returns empty list.

  The strategy is to find a minimal vertex coloring in the complement
  <math|G<rsup|c>> of <math|G> (note that these two graphs share the same set
  of vertices). Each set of equally colored vertices in <math|G<rsup|c>>
  corresponds to a clique in <math|G>. Therefore, the color classes of
  <math|G<rsup|c>> map to the elements <math|C<rsub|1>,\<ldots\>,C<rsub|k>>
  of a minimal clique cover in <math|G>.

  There is a special case in which <math|G> is triangle-free
  (i.e.<nbsp>contains no 3-cliques), which is computed separately in the
  algorithm. In that case, <math|G> contains only 1- and 2-cliques.
  Therefore, every clique cover in <math|G> consists of a set
  <math|M\<subset\>E> of matched edges together with the singleton cliques
  (i.e.<nbsp>the isolated vertices in <math|V> which remain unmatched). The
  total number of cliques in the cover is equal to
  <math|<around*|\||V|\|>-<around*|\||M|\|>>, hence to find a minimal cover
  one just needs to find a maximum matching in <math|G>, which can be done in
  polynomial time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(30,0.2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 83 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|0,22,24|]>,<around|[|1,14,17,20|]>,<around|[|2,25|]>,<around|[|3,10,16|]>,<around|[|4,28|]>,<around|[|5,19,29|]>,<around|[|6,27|]>,<around|[|7,8,11|]>,<around|[|9,12|]>,<around|[|13,23|]>,<around|[|15,26|]>,<around|[|18,21|]>|}>
      </equation*>
    </unfolded-io>

    \;

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(graph("octahedron"))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|3|3|cell-rborder|0ln>|<table|<row|<cell|1>|<cell|3>|<cell|6>>|<row|<cell|2>|<cell|4>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  The vertices of Petersen graph can be covered with five, but not with three
  cliques.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(graph("petersen"),3)
    <|unfolded-io>
      <\equation*>
        <around|[||]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(graph("petersen"),5)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|2>|<cell|3>>|<row|<cell|4>|<cell|9>>|<row|<cell|5>|<cell|7>>|<row|<cell|6>|<cell|8>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Clique cover number>

  The command <verbatim|clique_cover_number><index|<verbatim|clique_cover_number>>
  is used for computing the <hlink|<rigid|clique cover
  number>|https://en.wikipedia.org/wiki/Clique_cover> of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|clique_cover_number(G)>>>>>>

  <verbatim|clique_cover_number> takes an undirected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the minimum
  number of cliques in <math|G> needed to cover the vertex set <math|V>.
  (More precisely, it calls the <verbatim|<hlink|clique_cover|#minimum-covering>>
  command and returns the length of the output list.) This number, denoted by
  <math|\<theta\><around*|(|G|)>>, is equal to the chromatic number
  <math|\<chi\><around*|(|G<rsup|c>|)>> of the complement graph
  <math|G<rsup|c>> of <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover_number(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover_number(graph("soccerball"))
    <|unfolded-io>
      <\equation*>
        30
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover_number(random_graph(40,0.618))
    <|unfolded-io>
      <\equation*>
        7
      </equation*>
    </unfolded-io>
  </session>

  <section|Triangles in graphs>

  <subsection|Counting triangles>

  The command <verbatim|number_of_triangles><index|<verbatim|is_triangle_free>>
  is used for counting <hlink|<rigid|triangles>|https://en.wikipedia.org/wiki/Triangle_graph>
  in graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|number_of_triangles(G)>>>|<row|<cell|>|<cell|<verbatim|number_of_triangles(G,L)>>>>>>

  <verbatim|number_of_triangles> takes a graph <math|G> as its first,
  mandatory argument and returns the number <math|n> of 3-cliques in <math|G>
  if <math|G> is undirected resp.<nbsp>the number <math|m> of directed cycles
  of length 3 if <math|G> is directed. If an unassigned identifier <math|L>
  is given as the second argument, the triangles are also listed and stored
  to <math|L>. Note that triangle listing is supported only for undirected
  graphs.

  For undirected graphs the algorithm of <name|Schank> and <name|Wagner>
  <cite-detail|schank2|Algorithm <with|font-shape|italic|forward>>, improved
  by <name|Latapy><nbsp><cite|latapy>, is used, which runs in
  <math|O<around*|(|<around*|\||E|\|><rsup|3/2>|)>> time. For digraphs, the
  strategy is to compute the trace of <math|A<rsup|3>> where <math|A> is the
  adjacency matrix of <math|G> encoded in a sparse form. This algorithm
  requires <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(graph("tetrahedron"))
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph([1,2,3,4],%{[1,2],[1,4],[2,3],[2,4],[3,1],[4,3]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 6 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/tetrad.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(G)
    <|unfolded-io>
      <\equation*>
        2
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=sierpinski_graph(7,3,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1095 vertices and 2187
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(G)
    <|unfolded-io>
      <\equation*>
        972
      </equation*>
    </unfolded-io>
  </session>

  Petersen graph is triangle-free, i.e.<nbsp>contains no 3-cliques.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        0
      </equation*>
    </unfolded-io>
  </session>

  Counting triangles in undirected graphs is very fast, as illustrated by the
  following example.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(10^5,10^6)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100000 vertices and 1000000
        edges>
      </equation*>

      <timing|147 sec>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(G)
    <|unfolded-io>
      <\equation*>
        25315
      </equation*>

      <timing|1.62 sec>
    </unfolded-io>
  </session>

  To list all triangles in a graph, pass an unassigned identifier as the
  second argument. The triangles will be stored to it as a list of triples of
  vertices.\ 

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/octa.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_triangles(G,L)
    <|unfolded-io>
      <\equation*>
        8
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|3|3|cell-rborder|0ln>|<table|<row|<cell|2>|<cell|4>|<cell|5>>|<row|<cell|2>|<cell|4>|<cell|6>>|<row|<cell|2>|<cell|3>|<cell|5>>|<row|<cell|1>|<cell|4>|<cell|5>>|<row|<cell|2>|<cell|3>|<cell|6>>|<row|<cell|1>|<cell|4>|<cell|6>>|<row|<cell|1>|<cell|3>|<cell|5>>|<row|<cell|1>|<cell|3>|<cell|6>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Clustering coefficient><label|clustering-coefficient>

  The command <verbatim|clustering_coefficient><index|<verbatim|clustering_coefficient>>
  is used for computing the <hlink|<rigid|average clustering
  coefficient>|https://en.wikipedia.org/wiki/Clustering_coefficient#Network_average_clustering_coefficient>
  (or simply: clustering coefficient) of an undirected graph as well as the
  <hlink|<rigid|local clustering coefficient>|https://en.wikipedia.org/wiki/Clustering_coefficient#Local_clustering_coefficient>
  of a particular vertex in that graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|clustering_coefficient(G,[opt])>>>|<row|<cell|>|<cell|<verbatim|clustering_coefficient(G,v)>>>|<row|<cell|>|<cell|<verbatim|clustering_coefficient(G,v1,v2,..,vk)>>>|<row|<cell|>|<cell|<verbatim|clustering_coefficient(G,[v1,v2,..,vk])>>>>>>

  <verbatim|clustering_coefficient> takes one or two arguments, an undirected
  graph <math|G<around*|(|V,E|)>> and optionally a vertex <math|v\<in\>V> or
  a list/sequence of vertices <math|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|k>\<in\>V>.
  If <math|G> is the only argument, the clustering coefficient
  <math|c<around*|(|G|)>><nbsp><cite-detail|boot|pp.<nbsp>5> is returned.
  Otherwise, the local clustering coefficient <math|c<rsub|G><around*|(|v|)>>
  <cite-detail|boot|pp.<nbsp>4> of <math|v> resp.<nbsp>a list of local
  clustering coefficients of <math|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|k>>
  is returned. The second argument may also be one of the following options:

  <\description-dash>
    <verbatim|exact><item*|>The clustering coefficient
    <math|c<around*|(|G|)>> is returned as a rational number (by default it
    is a floating point number). Note that local clustering coefficient is
    always returned in exact form.

    <verbatim|approx><item*|>An approximation of the clustering coefficient
    <math|c<around*|(|G|)>>, lying within <math|0.5\<times\>10<rsup|-2>> of
    the exact value with probability <math|p=1-10<rsup|-5>>, is returned.
  </description-dash>

  In any case, the return value is\Vby definition\Va rational number in the
  range <math|<around*|[|0,1|]>>.

  The clustering coefficient of <math|G> is defined as the mean of
  <math|c<rsub|G><around*|(|v|)>>, <math|v\<in\>V>:\ 

  <\equation*>
    c<around*|(|G|)>=<frac|1|<around*|\||V|\|>>*<big|sum><rsub|v\<in\>V>c<rsub|G><around*|(|v|)>.
  </equation*>

  <math|c<around*|(|G|)>> can be interpreted as the probability that, for a
  randomly selected pair of incident edges <math|u v> and <math|v w> in
  <math|G>, the vertices <math|u> and <math|w> are connected. The number
  <math|c<rsub|G><around*|(|v|)>> is interpreted analogously but for a fixed
  <math|v\<in\>V>. It represents the probability that two neighbors of
  <math|v> are connected to each other.

  For example, assume that <math|G> represents a social network in which
  <math|u v\<in\>E> means that <math|u> and <math|v> are friends (which is a
  symmetric relation). In this context, <math|c<around*|(|v|)>> represents
  the probability that two friends of <math|v> are also friends of each
  other.

  The time complexity of computing <math|c<around*|(|G|)>> is
  <math|O<around*|(|<around*|\||E|\|><rsup|3/2>|)>>, whereas the algorithm of
  <name|Schank> and <name|Wagner><nbsp><cite-detail|schank|Algorithm<nbsp>1,
  pp.<nbsp>269> for approximating <math|c<around*|(|G|)>> runs in
  <math|O<around*|(|log <around*|\||V|\|>|)>> time.

  In addition, note that the command <verbatim|<hlink|random_graph|#random-graph>>
  is able to generate\Vusing a preferential attachment rule\Vrealistic random
  networks with adjustable clustering coefficient, which are suitable for
  testing purposes.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3],[2,4],[3,4],[4,1]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/ccg.eps|35%|||>>

  The command lines below compute <math|c<around*|(|G|)>>,
  <math|c<rsub|G><around*|(|1|)>> and <math|c<rsub|G><around*|(|2|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G,exact)
    <|unfolded-io>
      <\equation*>
        <frac|5|6>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G,1)
    <|unfolded-io>
      <\equation*>
        1
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G,2)
    <|unfolded-io>
      <\equation*>
        <frac|2|3>
      </equation*>
    </unfolded-io>
  </session>

  The next example demonstrates the performance of
  <verbatim|clustering_coefficient> on a large graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(25000,10,100)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 25000 vertices and 991473
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G)
    <|unfolded-io>
      <\equation*>
        0.635654820498
      </equation*>

      <timing|2.48 sec>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clustering_coefficient(G,approx)
    <|unfolded-io>
      <\equation*>
        0.635182159201
      </equation*>

      <timing|0.77 sec>
    </unfolded-io>
  </session>

  The probability that two neighbors of a vertex in <math|G> are connected is
  therefore about 64%.

  <subsection|Network transitivity>

  The command <verbatim|network_transitivity><index|<verbatim|network_transitivity>>
  is used for computing the <hlink|transitivity|https://en.wikipedia.org/wiki/Clustering_coefficient#Global_clustering_coefficient>
  (also called <strong|triangle density> or the <strong|global clustering
  coefficient>) of a network.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|network_transitivity(G)>>>>>>

  <verbatim|network_transitivity> takes a graph <math|G> as its only argument
  and returns the transitivity <math|T<around*|(|G|)>> of
  <math|G><nbsp><cite-detail|boot|pp.<nbsp>5>. By definition, it is a
  rational number in the range <math|<around*|[|0,1|]>>:

  <\equation*>
    T<around*|(|G|)>=<frac|3*N<rsub|triangles>|N<rsub|triplets>>.
  </equation*>

  <math|T<around*|(|G|)>> is a measure of transitivity of a non-symmetric
  relation between the vertices of a network. If <math|G> is a digraph, a
  <strong|triplet> in <math|G> is any directed path
  <math|<around*|(|v,w,z|)>> where <math|v,w,z\<in\>V>. For example, in a
  Twitter-like social network this could mean that <math|v> following
  <math|w> and <math|w> following <math|z>. The triplet
  <math|<around*|(|v,w,z|)>> is <strong|closed> if <math|v z\<in\>E>,
  i.e.<nbsp>if <math|v> also follows <math|z>
  <cite-detail|wasserman|pp.<nbsp>243>. A closed triplet is called a
  <strong|triangle>. If <math|G> is undirected, <math|N<rsub|triangles>> is
  the number of 3-cliques and <math|N<rsub|triplets>> is the number of
  two-edge paths in <math|V>.

  The complexity of computing <math|T<around*|(|G|)>> is
  <math|O<around*|(|\<Delta\><rsub|G>*<around*|\||E|\|>|)>> for digraphs,
  where <math|\<Delta\><rsub|G>> is the maximum vertex degree in <math|G>,
  resp.<nbsp><math|O<around*|(|<around*|\||E|\|><rsup|3/2>|)>> for undirected
  graphs.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3],[2,4],[3,4],[4,1]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      network_transitivity(G)
    <|unfolded-io>
      <\equation*>
        <frac|3|4>
      </equation*>
    </unfolded-io>
  </session>

  Observe that the above result is different than <math|c<around*|(|G|)>>
  obtained in Section<nbsp><reference|clustering-coefficient>. Hence
  <math|c<around*|(|G|)>\<neq\>T<around*|(|G|)>> in
  general<nbsp><cite-detail|boot|pp.<nbsp>5>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_digraph(10,20)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 10 vertices and 20 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <\center>
    <image|images/ntrans.eps|35%|||>
  </center>

  In the above digraph, the triplet <math|<around*|(|5,7,6|)>> is open while
  the triplet <math|<around*|(|7,6,4|)>> is closed. Triangles
  <math|<around*|(|2,5,9|)>> and <math|<around*|(|6,8,7|)>> are not closed by
  definition.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      network_transitivity(G)
    <|unfolded-io>
      <\equation*>
        <frac|5|33>
      </equation*>
    </unfolded-io>
  </session>

  The transitivity algorithms are suitable for large networks, as
  demonstrated in the examples below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_digraph(1000,500000)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 1000 vertices and 500000 arcs>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      nt:=network_transitivity(G):;
    <|unfolded-io>
      <timing|2.91 sec>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      evalf(nt)
    <|unfolded-io>
      <\equation*>
        0.500523736169
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=random_graph(30000,10,50)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30000 vertices and 1011266
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      evalf(network_transitivity(H))
    <|unfolded-io>
      <\equation*>
        0.137017372323
      </equation*>

      <timing|2.52 sec>
    </unfolded-io>
  </session>

  <section|Vertex coloring>

  To color vertices of a graph <math|G<around*|(|V,E|)>> means to assign to
  each vertex <math|v\<in\>V> a positive integer. Each integer represents a
  distinct color. The key property of <hlink|<rigid|graph
  coloring>|https://en.wikipedia.org/wiki/Graph_coloring#Vertex_coloring> is
  that the colors of a pair of adjacent vertices must be mutually different.
  Two different colorings of <math|G> may use different number of colors.

  <subsection|Greedy vertex coloring>

  The command <kbd|greedy_color><index|<kbd|greedy_color>> is used for
  coloring vertices of a graph in a greedy fashion.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|greedy_color(G)>>>|<row|<cell|>|<cell|<verbatim|greedy_color(G,p)>>>>>>

  <kbd|greedy_color> takes one mandatory argument, a graph
  <math|G<around*|(|V,E|)>>. Optionally, a permutation <math|p> of order
  <math|<around*|\||V|\|>> may be passed as the second argument. Vertices are
  colored one by one in the order specified by <math|p> (or in the default
  order if <math|p> is not given) such that each vertex gets the smallest
  available color. The list of vertex colors is returned in the order of
  <kbd|vertices(G)>.

  Generally, different choices of permutation <math|p> produce different
  colorings. The total number of different colors may not be the same each
  time. The complexity of the algorithm is
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      greedy_color(G)
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,1,2,3,2,1,3,3,2|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L:=greedy_color(G,randperm(10))
    <|unfolded-io>
      <\equation*>
        <around|[|1,2,1,4,3,4,1,3,2,2|]>
      </equation*>
    </unfolded-io>
  </session>

  Observe that a different number of colors is obtained by executing the last
  command line. To display the colored graph, input:

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_vertex(G,vertices(G),L),labels=false)
    </input>
  </session>

  <center|<image|images/color1.eps|35%|||>>

  The first six positive integers are always mapped to the standard
  <samp|Xcas> colors, as indicated in Table<nbsp><reference|tab:colors>.<\float|float|tbh>
    <big-table|<tabular|<tformat|<cwith|1|-1|2|-1|cell-halign|c>|<cwith|1|-1|1|1|cell-halign|r>|<cwith|1|-1|1|-1|cell-tborder|1ln>|<cwith|1|-1|1|-1|cell-bborder|1ln>|<cwith|1|-1|1|-1|cell-lborder|1ln>|<cwith|1|-1|1|-1|cell-rborder|1ln>|<table|<row|<cell|<em|value>>|<cell|1>|<cell|2>|<cell|3>|<cell|4>|<cell|5>|<cell|6>|<cell|7>>|<row|<cell|<em|color>>|<cell|red>|<cell|green>|<cell|yellow>|<cell|blue>|<cell|magenta>|<cell|cyan>|<cell|black>>>>>|<label|tab:colors>interpretation
    of abstract vertex/edge colors in <samp|Xcas>>
  </float> Note that the color 0 (black) and color 7 (white) are swapped; a
  vertex with color 0 is white (uncolored) and vertex with color 7 is black.
  Also note that <samp|Xcas> maps numbers greater than 7 to colors too, but
  the number of available colors is limited.

  <subsection|Minimal vertex coloring><label|minimal-coloring>

  A vertex coloring of <math|G> is <strong|minimal> (or <strong|optimal>) if
  the total number of used colors is minimal. To obtain such a coloring use
  the command <kbd|minimal_vertex_coloring><index|<kbd|minimal_vertex_coloring>>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|minimal_coloring(G)>>>|<row|<cell|>|<cell|<verbatim|minimal_coloring(G,sto)>>>>>>

  <kbd|minimal_vertex_coloring> takes one mandatory argument, a graph
  <math|G<around*|(|V,E|)>> where <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>.
  Optionally, a symbol <kbd|sto> may be passed as the second argument. The
  command returns the vertex colors <math|c<rsub|1>,c<rsub|2>,\<ldots\>,c<rsub|n>>
  in order of <kbd|vertices(G)> or, if the second argument is given, stores
  the colors as vertex attributes and returns the modified copy of <math|G>.

  <samp|Giac> requires the <hlink|<samp|GLPK>|https://www.gnu.org/software/glpk/>
  library to solve the minimal vertex coloring problem (<abbr|MVCP>), which
  is converted to the equivalent integer linear programming problem and
  solved by using the branch-and-bound method with specific branch/backtrack
  techniques <cite|diaz>. The lower resp.<nbsp>the upper bound for the number
  <math|n> of colors is obtained by finding a maximal clique (<math|n> cannot
  be lower than its cardinality) resp.<nbsp>by applying the heuristic
  proposed by <name|Brélaz> in<nbsp><cite|brelaz> (which will use at least
  <math|n> colors). Note that the algorithm performs some randomization when
  applying heuristics, so coloring a graph several times will not take the
  same amount of computation time in each instance, generally.

  In the following example, the Grötzsch graph is colored with the minimal
  number of colors by first finding the coloring and then assigning it to the
  graph by using the <verbatim|<hlink|highlight_vertex|#highlight-vertex>>
  command.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("grotzsch")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 11 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      coloring:=minimal_vertex_coloring(G)
    <|unfolded-io>
      <\equation*>
        <around|[|4,2,3,1,1,4,1,3,2,1,2|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_vertex(G,vertices(G),coloring),labels=false)
    </input>
  </session>

  <center|<image|images/grotzsch.eps|35%|||>>

  Solving <abbr|MVCP> for different graphs of exactly the same size (but
  which do not share the same edge structure) may take quite different time
  in each instance. Also note that, since the vertex coloring problem is
  NP<nbsp>hard, the algorithm may take exponential time on some graphs.

  <subsection|Chromatic number>

  The command <verbatim|chromatic_number><index|<verbatim|chromatic_number>>
  is used for exact computation or approximation of the
  <hlink|<rigid|chromatic number>|https://en.wikipedia.org/wiki/Graph_coloring#Vertex_coloring>
  of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|chromatic_number(G)>>>|<row|<cell|>|<cell|<verbatim|chromatic_number(G,c)>>>|<row|<cell|>|<cell|<verbatim|chromatic_number(G,approx
  or interval)>>>>>>

  <verbatim|chromatic_number> takes one mandatory argument, a graph
  <math|G<around*|(|V,E|)>>, and optionally a second argument. To obtain only
  upper and lower bound for the chromatic number (which is much faster than
  computing exactly) the option <verbatim|approx> or <verbatim|interval>
  should be passed as the second argument. Alternatively, an unassigned
  identifier <verbatim|c> is passed as the second argument; in that case the
  corresponding coloring will be stored to it in form of a list of colors of
  the individual vertices, ordered as in <verbatim|vertices(G)>.

  The command returns the chromatic number <math|\<chi\><rsub|G>> of the
  graph <math|G> in the case of exact computation. If the option
  <verbatim|approx> or <verbatim|interval> is given, an interval
  <verbatim|lb..ub> is returned, where <verbatim|lb> is the best lower bound
  and <verbatim|ub> the best upper bound for <math|\<chi\><rsub|G>> found by
  the algorithm.

  The strategy is call <verbatim|<hlink|minimal_vertex_coloring|#minimal-coloring>>
  in the case of exact computation. When approximating the chromatic number,
  the algorithm will establish the lower bound by finding a maximum clique.
  The timeout for this operation is set to 5 seconds as it can be time
  consuming. If no maximum clique is not found after that time, the largest
  clique found is used. Then, an upper bound is established by by using the
  heuristic proposed by <name|Brélaz> in<nbsp><cite|brelaz>. Obtaining the
  bounds for <math|\<chi\><rsub|G>> is usually very fast; however, their
  difference grows with <math|<around*|\||V|\|>>.

  Unless the input graph is sparse enough, the algorithm slows down
  considerably for, say, <math|<around*|\||V|\|>\<gtr\>40>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(graph("grotzsch"),cols)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      cols
    <|unfolded-io>
      <\equation*>
        <around|[|4,2,3,1,1,4,1,3,2,1,2|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(30,0.75)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 313 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(G)
    <|unfolded-io>
      <\equation*>
        10
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(300,0.05)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 300 vertices and 2196
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(G,approx)
    <|unfolded-io>
      <\equation*>
        4..7
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Mycielski graphs>

  The command <verbatim|mycielski><index|<verbatim|mycielski>> is used for
  constructing <hlink|<rigid|Mycielski graphs>|https://en.wikipedia.org/wiki/Mycielskian>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|mycielski(G)>>>>>>

  <verbatim|mycielski> takes an undirected graph <math|G<around*|(|V,E|)>> as
  its only argument and returns the corresponding Mycielski graph <math|M>
  (also called the <strong|Mycielskian> of <math|G>) with
  <math|2*<around*|\||V|\|>+1> vertices and
  <math|3*<around*|\||E|\|>+<around*|\||V|\|>> edges. If <math|G> is
  triangle-free then <math|M> is also triangle-free and
  <math|\<chi\><rsub|M>=\<chi\><rsub|G>+1>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=mycielski(P)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 21 vertices and 55 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      apply(number_of_triangles,[P,M])
    <|unfolded-io>
      <\equation*>
        <around|[|0,0|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(P)
    <|unfolded-io>
      <\equation*>
        3
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(M)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>
  </session>

  <verbatim|mycielski> can be applied iteratively, producing arbitrarily
  large graphs from the most simple ones. For example, Grötzsch graph is
  obtained as the Mycielskian of a cycle graph on 5 vertices, which is the
  Mycielskian of a path graph on two vertices.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=path_graph(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 2 vertices and 1 edge>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=mycielski(G1)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G2,cycle_graph(5))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G3:=mycielski(G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 11 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_isomorphic(G3,graph("grotzsch"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  All three graphs are triangle-free. Since it is obviously
  <math|\<chi\><rsub|G<rsub|1>>=2>, it follows
  <math|\<chi\><rsub|G<rsub|2>>=3> and <math|\<chi\><rsub|G<rsub|3>>=4>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      apply(chromatic_number,[G1,G2,G3])
    <|unfolded-io>
      <\equation*>
        <around|[|2,3,4|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|<math|k>-coloring>

  The command <verbatim|is_vertex_colorable><index|<verbatim|is_vertex_colorable>>
  is used for determining whether the vertices of a graph can be colored with
  at most <math|k><nbsp>colors.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_vertex_colorable(G,k)>>>|<row|<cell|>|<cell|<verbatim|is_vertex_colorable(G,k,c)>>>>>>

  <verbatim|is_vertex_colorable> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>>, a positive integer <math|k> and optionally an
  unassigned identifier <verbatim|c>. The command returns <verbatim|true> if
  <math|G> can be colored using at most <math|k> colors and <verbatim|false>
  otherwise. If the third argument is given, a coloring using at most
  <math|k> colors is stored to <verbatim|c> as a list of vertex colors, in
  the order of <verbatim|vertices(G)>.

  The strategy is to first apply a simple greedy coloring procedure which
  runs in linear time. If the number of required colors is greater than
  <math|k>, the heuristic proposed by <name|Brélaz> in<nbsp><cite|brelaz> is
  used, which runs in quadratic time. If the number of required colors is
  still larger than <math|k>, the algorithm attempts to find the chromatic
  number <math|\<chi\><rsub|G>> using <math|k> as the upper bound in the
  process.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("grotzsch")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 11 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_vertex_colorable(G,3)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_vertex_colorable(G,4)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(70,0.2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 70 vertices and 469 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(G,approx)
    <|unfolded-io>
      <\equation*>
        5..6
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_vertex_colorable(G,5)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>

      <timing|818 msec>
    </unfolded-io>
  </session>

  From the results of the last two command lines it follows
  <math|\<chi\><rsub|G>=6>. Finding <math|\<chi\><rsub|G>> by utilizing the
  next command line is simpler, but requires much more time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(G)
    <|unfolded-io>
      <\equation*>
        6
      </equation*>

      <timing|92.7 sec>
    </unfolded-io>
  </session>

  <section|Edge coloring>

  <subsection|Minimal edge coloring>

  The command <verbatim|minimal_edge_coloring><index|<verbatim|minimal_edge_coloring>>
  is used for finding a minimal <hlink|<rigid|coloring of
  edges>|https://en.wikipedia.org/wiki/Edge_coloring> in a graph, satisfying
  the following two conditions: any two mutually incident edges are colored
  differently and the total number <math|n> of colors is minimal. The theorem
  of <name|Vizing> <cite-detail|diestel|pp.<nbsp>103> implies that every
  simple undirected graph falls into one of two categories: <em|1> if
  <math|n=\<Delta\>> or <em|2> if <math|n=\<Delta\>+1>, where
  <math|\<Delta\>> is the maximum degree of the graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|minimal_edge_coloring(G)>>>|<row|<cell|>|<cell|<verbatim|minimal_edge_coloring(G,sto)>>>>>>

  <verbatim|minimal_edge_coloring> takes one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally the keyword <verbatim|sto>. If the
  latter is given, a minimal coloring is stored to the input graph (each edge
  <math|e\<in\>E> gets a color <math|c<rsub|e>> stored as an attribute) and a
  modified copy of <math|G> is returned. Else, the command returns a sequence
  of two objects: integer <verbatim|1> or <verbatim|2>, indicating the
  category, and the list of edge colors <math|c<rsub|e<rsub|1>>,c<rsub|e<rsub|2>>,\<ldots\>,c<rsub|e<rsub|m>>>
  according the order of edges <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|m>\<in\>E>
  as returned by the command <verbatim|<hlink|edges|#vertices-edges>>.

  The strategy is to find a minimal vertex coloring of the line graph of
  <math|G> by using the algorithm described in
  Section<nbsp><reference|minimal-coloring>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      minimal_edge_coloring(G)
    <|unfolded-io>
      <\equation*>
        2,<around|[|1,2,3,2,3,3,4,1,2,3,1,4,1,4,2|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=minimal_edge_coloring(graph("grotzsch"),sto)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 11 vertices and 20 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,labels=false)
    </input>
  </session>

  <center|<image|images/ecol.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(100,0.1)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 499 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      minimal_edge_coloring(G):;
    <|unfolded-io>
      <timing|20.24 sec>
    </unfolded-io>
  </session>

  <center|>

  <subsection|Chromatic index>

  The command <verbatim|chromatic_index><index|<verbatim|chromatic_index>> is
  used for computing the <hlink|<rigid|chromatic
  index>|https://en.wikipedia.org/wiki/Edge_coloring#Definitions> of an
  undirected graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|chromatic_index(G)>>>|<row|<cell|>|<cell|<verbatim|chromatic_index(G,c)>>>>>>

  <verbatim|chromatic_index> takes one or two arguments, an undirected graph
  <math|G<around*|(|E,V|)>> and optionally an unassigned identifier
  <verbatim|c>. The command returns the minimal number
  <math|\<chi\><rprime|'><around*|(|G|)>> of colors needed to color each edge
  in <math|G> such that two incident edges never share the same color. If the
  second argument is given, it specifies the destination for storing the
  coloring in form of a list of colors according to the order of edges in
  <math|E> as returned by the command <verbatim|<hlink|edges|#vertices-edges>>.

  The example below demonstrates how to color the edges of a graph with
  colors obtained by passing unassigned identifier <verbatim|c> to
  <verbatim|chromatic_index> as the second argument.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_index(G,c)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,edges(G),c))
    </input>
  </session>

  <center|<image|images/ecol2.eps|35%|||>>

  Blanu²a snarks, the two graphs with 18 vertices found in 1946 by
  <name|Danilo Blanu²a>, were the second and third
  <hlink|snarks|https://en.wikipedia.org/wiki/Snark_(graph_theory)>
  discovered <cite|blanusa>. For almost fifty years, Petersen graph was the
  only known snark. The second Blanu²a snark is available in <samp|Giac> by
  passing the string <verbatim|"blanusa"> to the
  <verbatim|<hlink|graph|#graph>> command.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("blanusa")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 18 vertices and 27 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,labels=false)
    </input>
  </session>

  <\center>
    <image|images/blanusa.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      minimum_degree(G),maximum_degree(G)
    <|unfolded-io>
      <\equation*>
        3,3
      </equation*>
    </unfolded-io>
  </session>

  To prove that Blanu²a snark is bridgeless, it is enough to show that it is
  biconnected, since each endpoint of a bridge is an articulation point
  (unless being of degree 1).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_biconnected(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      girth(G)
    <|unfolded-io>
      <\equation*>
        5
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_index(G)
    <|unfolded-io>
      <\equation*>
        4
      </equation*>
    </unfolded-io>
  </session>

  <chapter|Traversing graphs>

  <section|Walks and tours>

  <subsection|Eulerian graphs>

  The command <verbatim|is_eulerian><index|<verbatim|is_eulerian>> is used
  for determining whether an undirected graph contains an
  <hlink|<rigid|Eulerian trail>|https://en.wikipedia.org/wiki/Eulerian_path>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_eulerian(G)>>>|<row|<cell|>|<cell|<verbatim|is_eulerian(G,T)>>>>>>

  <verbatim|is_eulerian> takes one or two arguments, an undirected graph
  <math|G<around*|(|V,E|)>> and optionally an unassigned identifier
  <verbatim|T>, and returns <verbatim|true> if <math|G> is Eulerian and
  <verbatim|false> otherwise. If the second argument is given, the
  corresponding Eulerian trail is stored to <verbatim|T>.

  A graph <math|G> is <strong|Eulerian> if it has a trail covering all its
  edges. Such a trail is called <strong|Eulerian trail>. An Eulerian trail
  may be closed, in which case it is called <strong|Eulerian cycle>. Note
  that every edge <math|e\<in\>E> must be visited, i.e.<nbsp>\Pstrolled
  through\Q, exactly once <cite-detail|godsil|pp.<nbsp>395>. The edge
  endpoints (i.e.<nbsp>the vertices in <math|G>) may, however, be visited
  more than once.

  The strategy is to apply <name|Hierholzer>'s algorithm for finding an
  Eulerian path<nbsp><cite|hierholzer>. It works by covering one cycle at a
  time in the input graph. The required time is
  <math|O<around*|(|<around*|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_eulerian(complete_graph(4))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_eulerian(complete_graph([1,2,3,4,5]),T); T
    <|unfolded-io>
      <\equation*>
        <text|true>,<around|[|1,2,3,4,1,5,2,4,5,3,1|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_eulerian(graph("tetrahedron"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_eulerian(graph("octahedron"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Hamiltonian graphs>

  The command <verbatim|is_hamiltonian><index|<verbatim|is_hamiltonian>> is
  used for checking hamiltonicity of an undirected graph. The command can
  also construct a <hlink|<rigid|Hamiltonian
  cycle>|https://en.wikipedia.org/wiki/Hamiltonian_path> in the input graph
  if the latter is Hamiltonian.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|is_hamiltonian(G)>>>|<row|<cell|>|<cell|<verbatim|is_hamiltonian(G,hc)>>>>>>

  <verbatim|is_hamiltonian> takes one or two arguments, an undirected graph
  <math|G<around*|(|V,E|)>> and optionally an unassigned identifier
  <verbatim|hc>. The command returns <verbatim|true> if <math|G> is
  Hamiltonian and <verbatim|false> otherwise. When failing to determine
  whether <math|G> is Hamiltonian or not, <verbatim|is_hamiltonian> returns
  <verbatim|undef>. If the second argument is given, a Hamiltonian cycle is
  stored to <verbatim|hc>.

  The strategy is to apply some hamiltonicity criteria presented by
  <name|DeLeon> <cite|melissa> before resorting to the definitive but NP-hard
  algorithm. If <math|G> is not biconnected, it is not Hamiltonian. Else, the
  criterion of <name|Dirac> is applied: if
  <math|\<delta\><around*|(|G|)>\<geqslant\><frac|<around*|\||V|\|>|2>>,
  where <math|\<delta\><around*|(|G|)>=min<around*|{|deg<around*|(|v|)>\<of\>v\<in\>V|}>>,
  then <math|G> is Hamiltonian. Else, if <math|G> is bipartite with vertex
  partition <math|V=V<rsub|1>\<cup\>V<rsub|2>> and
  <math|<around*|\||V<rsub|1>|\|>\<neq\><around*|\||V<rsub|2>|\|>>, then
  <math|G> is not Hamiltonian. Else, the criterion of <name|Ore> is applied:
  if <math|deg<around*|(|u|)>+deg<around*|(|v|)>\<geqslant\>n> holds for
  every pair <math|u,v> of non-adjacent vertices from <math|V>, then <math|G>
  is Hamiltonian. Else, the theorem of <name|Bondy> and <name|Chvátal> is
  applied: if the closure <math|cl<around*|(|G|)>> of <math|G> (obtained by
  finding a pair <math|u,v> of non-adjacent vertices from <math|V> such that
  <math|deg<around*|(|u|)>+deg<around*|(|v|)>\<geqslant\>n>, adding a new
  edge <math|u v> to <math|E> and repeating the process until exhaustion) is
  Hamiltonian, then <math|G> is Hamiltonian. (Note that in this case the
  previously tried criteria are applied to <math|cl<around*|(|G|)>>; since
  the vertex degrees in <math|cl<around*|(|G|)>> are generally higher than
  those in <math|G>, the probability of success also rises.) Else, if the
  edge density of <math|G> is large enough, the criterion of <name|Nash> and
  <name|Williams> is applied: if <math|\<delta\><around*|(|G|)>\<geqslant\>max<around*|{|<frac|n+2|3>,\<beta\>|}>>,
  where <math|\<beta\>> is the independence number of <math|G>, then <math|G>
  is Hamiltonian. If all of the above criteria fail, the command
  <verbatim|<hlink|traveling_salesman|#traveling-salesman>> is called, either
  to find a Hamiltonian cycle in <math|G> or to determine that none exist.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_hamiltonian(graph("soccerball"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_hamiltonian(graph("octahedron"),hc)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(graph("octahedron"),hc))
    </input>
  </session>

  <\center>
    <image|images/tsp5.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_hamiltonian(graph("herschel"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_hamiltonian(graph("petersen"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_hamiltonian(hypercube_graph(9))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>

      <timing|6.04 sec>
    </unfolded-io>
  </session>

  <section|Optimal routing>

  <subsection|Shortest unweighted paths>

  The command <with|font-family|tt|shortest_path><index|<with|font-family|tt|shortest_path>>
  is used for finding shortest paths in unweighted graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|shortest_path(G,s,t)>>>|<row|<cell|>|<cell|<verbatim|shortest_path(G,s,T)>>>>>>

  <with|font-family|tt|shortest_path> takes three arguments: an undirected
  unweighted graph <math|G<around|(|V,E|)>>, the source vertex
  <math|s\<in\>V> and the target vertex <math|t\<in\>V> or a list <math|T> of
  target vertices. The shortest path from source to target is returned. If
  more targets are specified, the list of shortest paths from the source to
  each of these vertices is returned.

  The strategy is to run breadth-first traversal on the graph <math|G>
  starting from the source vertex <math|s>. The complexity of the algorithm
  is therefore <math|O*<around|(|<around|\||V|\|>+<around|\||E|\|>|)>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("dodecahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      shortest_path(G,1,16)
    <|unfolded-io>
      <\equation*>
        <around|[|1,6,11,16|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      paths:=shortest_path(G,1,[16,19])
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,6,11,16|]>,<around|[|1,5,10,14,19|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=highlight_trail(G,paths,[red,green])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/short.eps|35%|||>>

  <subsection|Cheapest weighted paths><label|cheapest-paths>

  The commands <with|font-family|tt|dijkstra><index|<with|font-family|tt|dijkstra>>
  and <verbatim|bellman_ford><index|<verbatim|bellman_ford>> are used for
  finding cheapest paths in weighted (directed) graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|dijkstra(G,s,t)>>>|<row|<cell|>|<cell|<verbatim|dijkstra(G,s,T)>>>|<row|<cell|>|<cell|<verbatim|bellman_ford(G,s,t)>>>|<row|<cell|>|<cell|<verbatim|bellman_ford(G,s,T)>>>>>>

  <with|font-family|tt|dijkstra> and <verbatim|bellman_ford> both take two or
  three arguments: a weighted (di)graph <math|G<around|(|V,E|)>>, a vertex
  <math|s\<in\>V> and optionally a vertex <math|t\<in\>V> or list <math|T> of
  vertices in <math|V>. It returns the cheapest path from <math|s> to
  <math|t> or, if more target vertices are given, the list of such paths to
  each target vertex <math|t\<in\>T>. If no target vertex is specified, all
  vertices in <math|V\<setminus\><around|{|s|}>> are assumed to be targets.
  If <verbatim|dijkstra> is used, the weights of edges in <math|E> must all
  be nonnegative. <verbatim|bellman_ford> accepts negative weights, but does
  not work if the input graph contains negative cycles (in which the weights
  of the corresponding edges sum up to a negative value).

  A cheapest path from <math|s> to <math|t> is represented with a list
  <with|font-family|tt|[[v1,v2,...,vk],c]> where the first element consists
  of path vertices with <math|v<rsub|1>=s> and <math|v<rsub|k>=t>, while the
  second element <math|c> is the weight (cost) of that path, equal to the sum
  of weights of its edges.

  <verbatim|dijsktra> computes the cheapest path using <name|Dijkstra>'s
  <hlink|algorithm|https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm>
  which runs in <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>>
  time<nbsp><cite|dijkstra>. <verbatim|bellman_ford> uses somewhat slower
  <hlink|algorithm|https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm#CITEREFShimbel1955>
  by <name|Bellman> and <name|Ford> (see <cite|bellman> and<nbsp><cite|ford>)
  which runs in <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>>
  time but in turn imposes less requirements upon its input.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[[1,2],1],[[1,6],3],[[2,3],3],[[3,4],7],[[4,5],3],[[5,6],3]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 6 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      res:=dijkstra(G,1,4)
    <|unfolded-io>
      <\equation*>
        <around|[|<around|[|1,6,5,4|]>,9|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(G,res[0]))
    </input>
  </session>

  <\center>
    <image|images/dijkstra.eps|35%|||>
  </center>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      dijkstra(G,1)
    <|unfolded-io>
      <\equation*>
        <around|[|<around|[|1|]>,0|]>,<around|[|<around|[|1,2|]>,1|]>,<around|[|<around|[|1,6|]>,3|]>,<around|[|<around|[|1,2,3|]>,4|]>,<around|[|<around|[|1,6,5,4|]>,9|]>,<around|[|<around|[|1,6,5|]>,6|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Traveling salesman problem><label|traveling-salesman>

  The command <verbatim|traveling_salesman><index|<verbatim|traveling_salesman>>
  is used for solving <hlink|<rigid|traveling salesman
  problem>|https://en.wikipedia.org/wiki/Travelling_salesman_problem>
  (<abbr|TSP>)<\footnote>
    For the details on traveling salesman problem and a historical overview
    see <cite|cook>.
  </footnote>.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|traveling_salesman(G,[opts])>>>|<row|<cell|>|<cell|<verbatim|traveling_salesman(G,M,[opts])>>>>>>

  <verbatim|traveling_salesman> takes the following arguments: an undirected
  graph <math|G<around*|(|V,E|)>>, a weight matrix <math|M> (optional) and a
  sequence of options (optional). The supported options are <verbatim|approx>
  and <verbatim|vertex_distance>.

  If the input graph <math|G> is unweighted and <math|M> is not specified, a
  Hamiltonian cycle (tour) is returned (the adjacency matrix of <math|G> is
  used for the edge weights). If <math|G> is weighted, two objects are
  returned: the optimal value for the traveling salesman problem and a
  Hamiltonian cycle which achieves the optimal value. If <math|M> is given
  and <math|G> is unweighted, <math|M> is used as the weight matrix for
  <math|G>.

  If the option <verbatim|vertex_distance> is passed and <math|M> is not
  specified, then for each edge <math|e\<in\>E> the Euclidean distance
  between its endpoints is used as the weight of <math|e>. Therefore it is
  required for each vertex in <math|G> to have a predefined position.

  If the option <verbatim|approx> is passed, a near-optimal tour is returned.
  In this case it is required that <math|G> is a complete weighted graph. For
  larger graphs, this is significantly faster than finding optimal tour.
  Results thus obtained are usually only a few percent larger than the
  corresponding optimal values, despite the fact that the reported guarantee
  is generally much weaker (around 30%).

  The strategy is to formulate <abbr|TSP> as a linear programming problem and
  to solve it by branch-and-cut method, applying the hierarchical clustering
  method of <name|Pferschy> and <name|Stan¥k><nbsp><cite|pferschy> to
  generate subtour elimination constraints. The branching rule is implemented
  according to <name|Padberg> and <name|Rinaldi><nbsp><cite|padberg>. In
  addition, the algorithm combines the method of
  <name|Christofides><nbsp><cite|christofides>, the method of farthest
  insertion and a variant of the powerful tour improvement heuristic
  developed by <name|Lin> and <name|Kernighan><nbsp><cite|helsgaun> to
  generate near-optimal feasible solutions during the branch-and-cut process.

  For Euclidean <abbr|TSP> instances, i.e.<nbsp>in cases when <math|G> is a
  complete graph with vertex distances as the edge weights, the algorithm
  usually finishes in a few seconds for <abbr|TSP> with up to, say, 42
  cities. For problems with 100 or more cities, the option <verbatim|approx>
  is recommended as finding the optimal value takes a long time. Note that
  <abbr|TSP> is <abbr|NP>-hard, meaning that no polynomial time algorithm is
  known. Hence the algorithm may take exponential time to find the optimum in
  some instances.

  The following example demonstrates finding a Hamiltonian cycle in the
  truncated icosahedral (\Psoccer ball\Q) graph. The result is visualized by
  using <verbatim|<hlink|highlight_trail|#highlight-trail>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("soccerball")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 90 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(G,traveling_salesman(G)),labels=false)
    </input>
  </session>

  <center|<image|images/tsp1.eps|35%|||>>

  A matrix may be passed alongside an undirected graph to specify the edge
  weights. The alternative is to pass a weighted graph as the single
  argument.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=randmatrix(6,6,25)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|3|3|cell-halign|c>|<cwith|1|-1|4|4|cell-halign|c>|<cwith|1|-1|5|5|cell-halign|c>|<cwith|1|-1|6|6|cell-halign|c>|<cwith|1|-1|6|6|cell-rborder|0ln>|<table|<row|<cell|7>|<cell|13>|<cell|15>|<cell|10>|<cell|17>|<cell|6>>|<row|<cell|17>|<cell|7>|<cell|23>|<cell|3>|<cell|17>|<cell|5>>|<row|<cell|15>|<cell|24>|<cell|19>|<cell|15>|<cell|20>|<cell|24>>|<row|<cell|3>|<cell|16>|<cell|10>|<cell|18>|<cell|1>|<cell|3>>|<row|<cell|9>|<cell|20>|<cell|9>|<cell|2>|<cell|19>|<cell|15>>|<row|<cell|17>|<cell|8>|<cell|19>|<cell|20>|<cell|15>|<cell|15>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      c,t:=traveling_salesman(G,M)
    <|unfolded-io>
      <\equation*>
        57.0,<around|[|4,5,2,3,1,6,4|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(make_weighted(G,M),t))
    </input>
  </session>

  <center|<image|images/tsp4.eps|35%|||>>

  In the next example, an instance of Euclidean <abbr|TSP> with 42 cities is
  solved to optimality. The vertex positions are pairs of integers randomly
  chosen on the grid <math|<around*|[|0,1000|]>\<times\><around*|[|0,1000|]>\<in\>\<bbb-Z\><rsup|2>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=set_vertex_positions(complete_graph(42),[randvector(2,1000)$(k=1..42)])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 42 vertices and 861 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      c,t:=traveling_salesman(G,vertex_distance):;
    <|unfolded-io>
      <timing|10.01 sec>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(subgraph(G,trail2edges(t)),labels=false)
    </input>
  </session>

  <center|<image|images/tsp2.eps|35%|||>>

  For large instances of Euclidean <abbr|TSP> the <verbatim|approx> option
  may be used, as in the following example with 555 cities.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=set_vertex_positions(complete_graph(555),[randvector(2,10000)$(k=1..555)])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 555 vertices and 153735
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      ac,t:=traveling_salesman(H,vertex_distance,approx):;
    <|unfolded-io>
      <timing|49.34 sec>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(subgraph(H,trail2edges(t)))
    </input>
  </session>

  <center|<image|images/tsp3.eps|35%|||>>

  Near-optimal tours produced by the <verbatim|approx> option are usually
  only slightly more expensive than the optimal ones. For example, a
  sub-optimal tour for the previous instance <math|G> with 42 cities is
  obtained by the following command.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      ac,st:=traveling_salesman(G,vertex_distance,approx):;
    <|unfolded-io>
      The tour cost is within 28% of the optimal value
    </unfolded-io>
  </session>

  Although it is guaranteed that the near-optimal cost <verbatim|ac> is for
  at most 28% larger than <verbatim|c> (the optimum), the actual relative
  difference is smaller than 3%, as computed below.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      100*(ac-c)/c
    <|unfolded-io>
      <\equation*>
        2.7105821877
      </equation*>
    </unfolded-io>
  </session>

  <section|Spanning trees>

  <subsection|Construction of spanning trees>

  The command <with|font-family|tt|spanning_tree><index|<with|font-family|tt|spanning_tree>>
  is used for construction of <hlink|<rigid|spanning
  trees>|https://en.wikipedia.org/wiki/Spanning_tree> in graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|spanning_tree(G)>>>|<row|<cell|>|<cell|<verbatim|spanning_tree(G,r)>>>>>>

  <verbatim|spanning_tree> takes one or two arguments, an undirected graph
  <math|G<around*|(|V,E|)>> and optionally a vertex <math|r\<in\>V>. It
  returns the spanning tree <math|T> (rooted in <math|r>) of <math|G>,
  obtained by depth-first traversal in <math|O<around|(|<around|\||V|\|>+<around|\||E|\|>|)>>
  time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      P:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T1:=spanning_tree(P)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(P)
    </input>
  </session>

  <center|<image|images/span1.eps|35%|||>>

  By extracting <math|T<rsub|1>> from <math|P> as a subgraph, it inherits
  vertex positions from <math|P>.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(subgraph(P,edges(T1)))
    </input>
  </session>

  <center|<image|images/span2.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T2:=spanning_tree(P,4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 9 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(T1), edges(T2)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|1>|<cell|2>>|<row|<cell|2>|<cell|3>>|<row|<cell|3>|<cell|4>>|<row|<cell|4>|<cell|9>>|<row|<cell|5>|<cell|7>>|<row|<cell|5>|<cell|8>>|<row|<cell|6>|<cell|8>>|<row|<cell|6>|<cell|9>>>>>|)>,<around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|0>|<cell|1>>|<row|<cell|0>|<cell|4>>|<row|<cell|1>|<cell|2>>|<row|<cell|2>|<cell|3>>|<row|<cell|3>|<cell|8>>|<row|<cell|5>|<cell|7>>|<row|<cell|5>|<cell|8>>|<row|<cell|6>|<cell|9>>|<row|<cell|7>|<cell|9>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Minimal spanning tree>

  The command <with|font-family|tt|minimal_spanning_tree><index|<with|font-family|tt|minimal_spanning_tree>>
  is used for obtaining <hlink|<rigid|minimal spanning
  trees>|https://en.wikipedia.org/wiki/Minimum_spanning_tree> in undirected
  graphs.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|minimal_spanning_tree(G)>>>>>>

  <with|font-family|tt|minimal_spanning_tree> takes an undirected graph
  <math|G<around|(|V,E|)>> as its only argument and returns its minimal
  spanning tree as a graph. If <math|G> is not weighted, it is assumed that
  the weight of each edge in <math|E> is equal to 1.

  The strategy is to apply <name|Kruskal>'s algorithm which runs in
  <math|O<around|(|<around|\||E|\|>*log <around|\||V|\|>|)>> time.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      A:=[[0,1,0,4,0,0],[1,0,1,0,4,0],[0,1,0,3,0,1],[4,0,3,0,1,0],[0,4,0,1,0,4],[0,0,1,0,4,0]]:;
    </input>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(A)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 6 vertices and 8 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=minimal_spanning_tree(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 6 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(T,weights)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|<around|[|0,1|]>,1|]>,<around|[|<around|[|1,2|]>,1|]>,<around|[|<around|[|2,5|]>,1|]>,<around|[|<around|[|2,3|]>,3|]>,<around|[|<around|[|3,4|]>,1|]>|}>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_subgraph(G,T))
    </input>
  </session>

  <center|<image|images/span3.eps|35%|||>>

  <subsection|Counting the spanning trees in a
  graph><label|number-of-spanning-trees>

  The command <verbatim|number_of_spanning_trees><index|<verbatim|number_of_spanning_trees>>
  is used for <hlink|<rigid|counting spanning
  trees>|https://en.wikipedia.org/wiki/Spanning_tree#Counting_spanning_trees>
  in a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|number_of_spanning_trees(G)>>>>>>

  <verbatim|number_of_spanning_trees> takes an undirected graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the total number
  <math|n> of (labeled) spanning trees in <math|G>.

  The strategy is to use <hlink|<rigid|<name|Kirchhoff>'s
  Theorem>|https://en.wikipedia.org/wiki/Kirchhoff%27s_theorem><nbsp><cite-detail|west|Theorem<nbsp>2.2.12,
  pp.<nbsp>86>. The number of spanning trees is equal to the first principal
  minor of the <hlink|<rigid|Laplacian matrix>|#laplacian-matrix> of
  <math|G>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_spanning_trees(graph("octahedron"))
    <|unfolded-io>
      <\equation*>
        384
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_spanning_trees(graph("dodecahedron"))
    <|unfolded-io>
      <\equation*>
        5184000
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_spanning_trees(hypercube_graph(4))
    <|unfolded-io>
      <\equation*>
        42467328
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      number_of_spanning_trees(graph("soccerball"))
    <|unfolded-io>
      <\equation*>
        375291866372898816000
      </equation*>
    </unfolded-io>
  </session>

  \;

  <chapter|Visualizing graphs>

  <section|Drawing graphs>

  The <kbd|draw_graph><index|<kbd|draw_graph>> command<label|draw-graph> is
  used for visualizing graphs. It is capable to produce a drawing of a graph
  using one of the several built-in methods.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|draw_graph(G)>>>|<row|<cell|>|<cell|<verbatim|draw_graph(G,opts)>>>>>>

  <subsection|Overview>

  <with|font-family|tt|draw_graph> takes one or two arguments, the mandatory
  first one being a graph <math|G<around|(|V,E|)>>. This command assigns 2D
  or 3D coordinates to each vertex <math|v\<in\>V> and produces a visual
  representation of <math|G> based on these coordinates. The second
  (optional) argument is a sequence of options. Each option is one of the
  following.

  <\description-dash>
    <with|font-family|tt|labels=true> or <with|font-family|tt|false><item*|>Control
    the visibility of vertex labels and edge weights (by default
    <with|font-family|tt|true>, i.e.<nbsp>the labels and weights are
    displayed).

    <with|font-family|tt|spring><item*|>Apply a multilevel force-directed
    algorithm.

    <with|font-family|tt|tree[=r> or <with|font-family|tt|[r1,r2,...]]><item*|>Draw
    a tree or forest <math|G>, optionally specifying the root node for each
    tree (by default the first node is used).

    <with|font-family|tt|bipartite><item*|>Draw a bipartite graph <math|G>,
    separating the vertex partitions from one another.

    <with|font-family|tt|circle[=L]> or <with|font-family|tt|convexhull[=L]><item*|>Draw
    a graph <math|G> by spreading the <with|font-shape|italic|hull vertices>
    from list <math|L\<subset\>V> (assuming <math|L=V> by default) across the
    unit circle and putting all other vertices in origin, subsequently
    applying a force-directed vertex placement algorithm to generate the
    layout while keeping the hull vertices fixed.

    <with|font-family|tt|planar> or <with|font-family|tt|plane><item*|>Draw a
    planar graph <math|G> using a force-directed algorithm.

    <with|font-family|tt|plot3d><item*|>Draw a connected graph <math|G> as if
    the <with|font-family|tt|spring> option was enabled, but with vertex
    positions in <abbr|3D> instead of <abbr|2D>.
  </description-dash>

  If an unassigned identifier is passed as an argument, it is used as the
  destination for storing the computed vertex positions as a list.

  The style options <with|font-family|tt|spring>, <with|font-family|tt|tree>,
  <with|font-family|tt|circle>, <with|font-family|tt|planar> and
  <with|font-family|tt|plot3d> cannot be mixed, i.e.<nbsp>at most one can be
  specified. The option <with|font-family|tt|labels> may be combined with any
  of the style options. Note that edge weights will not be displayed when
  using <with|font-family|tt|plot3d> option when drawing a weighted graph.

  When no style option is specified, the algorithm first checks if the graph
  <math|G> is a tree or if it is bipartite, in which cases it is drawn
  accordingly. Otherwise, the graph is drawn as if the option
  <with|font-family|tt|circle> was specified.

  Tree, circle and bipartite drawings can be obtained in linear time with a
  very small overhead, allowing graphs to be drawn quickly no matter the
  size. The force-directed algorithms are more expensive and operating in the
  time which is quadratic in the number of vertices. Their performance is,
  nevertheless, practically instantaneous for graphs with several hundreds of
  vertices (or less).

  <subsection|Drawing disconnected graphs>

  When the input graph has two or more connected components, each component
  is drawn separately and the drawings are subsequently arranged such that
  the bounding box of the whole drawing has the smallest perimeter under
  condition that as little space as possible is wasted inside the box.

  For example, the command lines below draw a sparse random planar graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_planar_graph(100,0.9,0)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 74 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar)
    </input>
  </session>

  <center|<image|images/sparse.eps|30%|||>>

  <subsection|Spring method>

  When the option <kbd|spring> is specified, the input graph is drawn using
  the force-directed algorithm described in<nbsp><cite|hu> (for an example of
  such drawing see Figure<nbsp><reference|st53>). The idea, originally due to
  <name|Fruchterman> and <name|Reingold> <cite|fruchterman>, is to simulate
  physical forces in a spring-electrical model where the vertices and edges
  represent equally charged particles and springs connecting them,
  respectively.

  In a spring-electrical model, each vertex is being repulsed by every other
  vertex with a force inversely proportional to the distance between them. At
  the same time, it is attracted to each of its neighbors with a force
  proportional to the square of the distance. Assuming that <math|x<rsub|v>>
  is the vector representing the position of the vertex <math|v\<in\>V>, the
  total force <math|F<rsub|v>> applied to <math|v> is equal to

  <\equation*>
    F<rsub|v>=<big|sum><rsub|w\<in\>V\<setminus\><around*|{|v|}>>-<frac|C*K<rsup|2>|<around*|\<\|\|\>|x<rsub|v>-x<rsub|w>|\<\|\|\>><rsup|2>>*<around*|(|x<rsub|v>-x<rsub|w>|)>+<big|sum><rsub|w\<in\>N<around*|(|v|)>><frac|<around*|\<\|\|\>|x<rsub|v>-x<rsub|w>|\<\|\|\>>|K>*<around*|(|x<rsub|v>-x<rsub|w>|)>,
  </equation*>

  where <math|N<around*|(|v|)>> is the set of neighbors of <math|v> and
  <math|C>, <math|K> are certain positive real constants (actually, <math|K>
  may be any positive number, it affects only the scaling of the entire
  layout). Applying the forces iteratively and updating vertex positions in
  each iteration (starting from a random layout) leads the system to the
  state of minimal energy. By applying a certain \Pcooling\Q scheme to the
  model which cuts down the force magnitude in each iteration. the layout
  \Pfreezes\Q after a number of iterations large enough to achieve the
  minimal energy state.

  The force-directed method is computationally expensive and for larger
  graphs the pleasing layout cannot be obtained most of the time since the
  algorithm, starting with a random initial layout, gets easily \Pstuck\Q in
  a local energy minimum. To avoid this a multilevel scheme is applied. The
  input graph is iteratively coarsened, either by removing the vertices from
  a maximal independent vertex set or by contracting the edges of a maximal
  matching in each iteration. Each coarsening level is processed by the
  force-directed algorithm, starting from the deepest (coarsest) one and
  \Plifting\Q the obtained layout to the first upper level, using it as the
  initial layout for that level. The lifting done using a prolongation matrix
  technique described in<nbsp><cite|hu2>. To support drawing large graphs
  (with, say, 1000 vertices or more), the matrices used in the lifting
  process are stored in sparse form. The multilevel scheme also speeds up the
  layout process significantly.

  If the structure of the input graph is symmetric, a layout obtained by
  using a force-directed method typically reveals these symmetries, which is
  a unique property among graph drawing algorithms. To make the symmetries
  more prominent, the layout is rotated such that the axis, with respect to
  which the layout exhibits the largest <em|symmetry score>, becomes
  vertical. Because symmetry detection is computationally quite expensive (up
  to <math|O<around*|(|<around*|\||V|\|><rsup|7>|)>> when using the symmetry
  measure of <name|Purchase><nbsp><cite|welch>, for example), the algorithm
  accounts only the convex hull and the barycenter of the layout, which may
  not always be enough to produce the optimal result. Nevertheless, this
  approach is fast and works (most of the time) for highly symmetrical
  graphs.

  For example, the following command lines produce a drawing of the tensor
  product of two graphs using the force-directed algorithm.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph(trail(1,2,3,4,5,2))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=star_graph(3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=tensor_product(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/tensorproduct.eps|35%|||>>

  The following example demonstrates drawing a much larger graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=sierpinski_graph(5,4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1024 vertices and 2046
        edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(S,spring)
    </input>
  </session>

  <center|<image|images/sg54.eps|35%|||>>

  Note that vertex labels are automatically suppressed because of the large
  number of vertices. On our system, the algorithm took less than two seconds
  to produce the layout.

  The spring method is also used for creating <abbr|3D> graph layouts, which
  are obtained by passing the option <verbatim|plot3d> to the
  <verbatim|draw_graph> command.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(graph("soccerball"),plot3d,labels=false)
    </input>
  </session>

  <center|<image|images/ball.eps|45%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=graph("icosahedron"):; G2:=graph("dodecahedron"):;
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G1:=highlight_edges(G1,edges(G1),red)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 12 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G2:=highlight_edges(G2,edges(G2),magenta)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=disjoint_union(G1,G2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 32 vertices and 60 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,plot3d,labels=false)
    </input>
  </session>

  <center|<image|images/3d.eps|35%|||>>

  <subsection|Drawing trees>

  When the <verbatim|tree[=r]> option is specified and the input graph
  <math|G> is a tree (and <math|r\<in\>V>), it is drawn using a fast but
  simple node positioning algorithm inspired by the well-known algorithm of
  <name|Walker> <cite|walker>, using the first vertex (or the vertex
  <math|r>) as the root node. When drawing a rooted tree, one usually
  requires the following aesthetic properties <cite|buchheim>.

  <\description-aligned>
    <item*|A1>The layout displays the hierarchical structure of the tree,
    i.e.<nbsp>the <math|y>-coordinate of a node is given by its level.

    <item*|A2>The edges do not cross each other.

    <item*|A3><label|a3>The drawing of a sub-tree does not depend on its
    position in the tree, i.e.<nbsp>isomorphic sub-trees are drawn
    identically up to translation.

    <item*|A4>The order of the children of a node is displayed in the
    drawing.

    <item*|A5>The algorithm works symmetrically, i.e.<nbsp>the drawing of the
    reflection of a tree is the reflected drawing of the original tree.
  </description-aligned>

  The algorithm implemented in <samp|Giac> generally satisfies all the above
  properties but<nbsp><reference|a3>. Instead, it tries to spread the inner
  sub-trees evenly across the available horizontal space. It works by
  organizing the structure of the input tree into levels by using depth-first
  search and laying out each level subsequently, starting from the deepest
  one and climbing up to the root node. In the end, another depth-first
  traversal is made, shifting the sub-trees horizontally to avoid
  intersections between their edges. The algorithm runs in
  <math|O<around*|(|<around*|\||V|\|>|)>> time and uses the minimum of
  horizontal space to draw the tree with respect to the specified root node
  <math|r>.

  For example, the following command line draws a random free unlabeled tree
  on 100 nodes.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(random_tree(100))
    </input>
  </session>

  <center|<image|images/tree.eps|35%|||>>

  <subsection|Drawing planar graphs>

  The algorithm implemented in <samp|Giac> which draws planar graphs uses
  augmentation techniques to extend the input graph <math|G> to a graph
  <math|G<rprime|'>>, which is homeomorphic to some triconnected graph, by
  adding temporary edges. The augmented graph <math|G<rprime|'>> is then
  drawn using <name|Tutte>'s barycentric method (see <cite|tutte> and
  <cite-detail|godsil|pp.<nbsp>293>) which puts each vertex in the barycenter
  of its neighbors. It is guaranteed that a (non-strict) convex drawing will
  be produced, without edge crossings. In the end, the duplicate of the outer
  face and the temporary edges inserted during the augmentation stage are
  removed.

  <name|Tutte>'s algorithm requires that the vertices of the chosen outer
  face are initially fixed somewhere the boundary of a convex polygon. In
  addition, to produce a more flexible layout, the outer face is duplicated
  such that the subgraph induced by the vertices on both the outer face and
  its duplicate is a prism graph. Then only the duplicates of the outer face
  vertices are fixed, allowing the outer face itself to take a more natural
  shape. The duplicate of the outer face is removed after a layout is
  produced.

  The augmentation process consists of two parts. Firstly, the input graph
  <math|G> is decomposed into biconnected components (blocks) using the
  depth-first search <cite-detail|gibbons|pp.<nbsp>25>. Each block is then
  decomposed into faces (represented by cycles of vertices) using
  <name|Demoucron>'s algorithm (see <cite-detail|gibbons|pp.<nbsp>88> and
  <cite|myrwold>). Embeddings obtained for each blocks are then combined by
  adding one temporary edge for each articulation point, joining the two
  corresponding blocks. Figure<nbsp><reference|blockjoin> shows the outer
  faces of two blocks <math|B<rsub|1>> and <math|B<rsub|2>>, connected by an
  articulation point (cut vertex). The temporary edge (shown in green) is
  added to join <math|B<rsub|1>> and <math|B<rsub|2>> into a single block.
  After \Pfolding up\Q the tree of blocks, the algorithm picks the largest
  face in the resulting biconnected graph to be the outer face of the planar
  embedding.<\float|float|tbh>
    <big-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<point|-7.02714|4.71764>|<point|-7.27902|3.87308>|<point|-6.21221|2.99889>|<with|color|green|line-width|2ln|<line|<point|-5.9307|3.7101>|<point|-5.32321|2.76182>>>|<point|-5.9307|3.7101>|<point|-6.36038|4.55465>|<point|-6.74562|2.13952>|<point|-6.0196|1.26533>|<point|-5.11577|0.998627>|<point|-4.83425|2.02098>|<point|-5.32321|2.76182>|<line|<point|-7.02714|4.71764>|<point|-6.36038|4.55465>|<point|-5.9307|3.7101>|<point|-6.21221|2.99889>|<point|-7.27902|3.87308>|<point|-7.02714|4.71764>>|<line|<point|-6.21221|2.99889>|<point|-6.74562|2.13952>|<point|-6.0196|1.26533>|<point|-5.11577|0.998627>|<point|-4.83425|2.02098>|<point|-5.32321|2.76182>|<point|-6.21221|2.99889>>|<text-at|<math|B<rsub|1>>|<point|-6.86415200423336|3.87308175684614>>|<text-at|<math|B<rsub|2>>|<point|-5.93069519777748|1.90245072099484>>|<text-at|temp.<nbsp>edge|<point|-6.02040613837809|4.26236936102659>>|<with|dash-style|11100|<line|<point|-4.92621811732097|4.05921748908586>|<point|-5.44174163249107|3.28041076862019>>>>>|<label|blockjoin>Joining
    two block by adding a temporary edge.>
  </float>

  The second part of the augmentation process consists of recursively
  decomposing each non-convex inner face into several convex polygons by
  adding temporary edges. An inner face <math|f=<around*|(|v<rsub|1>,\<ldots\>,v<rsub|n>|)>>
  is non-convex if there exist <math|k> and <math|l> such that
  <math|1\<leqslant\>k\<less\>l-1\<less\>n> and either <math|v<rsub|k>
  v<rsub|l>\<in\>E>, in which case the edge <math|v<rsub|k> v<rsub|l>> is a
  <em|chord> (see Figure<nbsp><reference|chordface> for an
  example)<\float|float|tbh>
    <big-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<with|color|green|line-width|2ln|<line|<point|-7.03767|3.2286>|<point|-6.0|3.6>>>|<point|-6|3.6>|<text-at|<math|f>|<point|-5.7|2.8>>|<with|color|red|line-width|2ln|<line|<point|-6.59316708559333|4.25095912157693>|<point|-4.25833443577193|4.01635467654452>>>|<point|-6.59316708559333|4.25095912157693>|<point|-4.91887154385501|2.01362614102394>|<point|-6.51908321206509|2.36922873395952>|<point|-7.0376703267628|3.22860166688715>|<with|color|green|line-width|2ln|<line|<point|-5.22142|3.45332>|<point|-4.18425056224368|2.8902797989152>>>|<point|-5.22142479163911|3.45331723772986>|<line|<point|-6.59316708559333|4.25095912157693>|<point|-7.0376703267628|3.22860166688715>|<point|-6.51908321206509|2.36922873395952>|<point|-4.91887154385501|2.01362614102394>|<point|-4.18425056224368|2.8902797989152>|<point|-4.25833443577193|4.01635467654452>|<point|-5.22142479163911|3.45331723772986>|<point|-6.0|3.6>|<point|-6.59316708559333|4.23614234687128>>|<point|-4.18425056224368|2.8902797989152>|<point|-4.25833443577193|4.01635467654452>|<text-at|chord|<point|-5.82891255457071|5.08316245535124>>|<with|dash-style|11100|<line|<point|-5.36484654054769|4.95510318825241>|<point|-5.3399589892843|4.31269017065749>>>>>|<label|chordface>A
    chorded face <math|f>.>
  </float> or there exists a face <math|g=<around*|(|w<rsub|1>,w<rsub|2>,\<ldots\>,v<rsub|k>,\<ldots\>,v<rsub|l>,\<ldots\>,w<rsub|m-1>,w<rsub|m>|)>>
  such that the vertices <math|v<rsub|k+1>,\<ldots\>,v<rsub|l-1>> are not
  contained in <math|g> (see Figure<nbsp><reference|touchface> for an
  example)<\float|float|tbh>
    <big-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<with|color|green|line-width|2ln|<line|<point|-7.47976|3.22362>|<point|-6.31419|3.16188>>>|<with|color|green|line-width|2ln|<line|<point|-5.73634|3.17669>|<point|-4.96587|4.11015>>>|<with|color|red|<line|<point|-7.12912|2.64329>|<point|-7.47975922741103|3.22362415663448>|<point|-7.61807|3.87308>|<point|-7.04022|4.45094>|<point|-5.95859|4.64355>|<point|-4.96587|4.11015>|<point|-5.14367|2.99889>|<point|-5.73634|3.17669>|<point|-6.31419|3.16188>|<point|-7.12912|2.64329>>>|<with|color|green|line-width|2ln|<line|<point|-7.24765|2.09507>|<point|-6.71425|2.33214>>>|<with|color|green|line-width|2ln|<line|<point|-5.57336|2.53957>|<point|-4.53618|2.33214>>>|<with|color|blue|<line|<point|-7.12912|2.64329>|<point|-7.24765|2.09507>|<point|-7.17357|1.39868>|<point|-6.37346|0.89491>|<point|-5.42519|0.835643>|<point|-4.62508|1.51721>|<point|-4.53618|2.33214>|<point|-5.14367|2.99889>|<point|-5.57336|2.53957>|<point|-6.03268|2.22842>|<point|-6.71425|2.33214>|<point|-7.12912|2.64329>>>|<point|-7.04022|4.45094>|<point|-7.61807|3.87308>|<point|-7.12912|2.64329>|<point|-6.31419|3.16188>|<point|-5.73634|3.17669>|<point|-5.14367|2.99889>|<point|-4.96587|4.11015>|<point|-5.95859|4.64355>|<point|-7.24765|2.09507>|<point|-7.17357|1.39868>|<point|-6.37346|0.89491>|<point|-5.42519|0.835643>|<point|-4.62508|1.51721>|<point|-4.53618|2.33214>|<point|-5.57336|2.53957>|<point|-6.03268|2.22842>|<point|-6.71425|2.33214>|<text-at|<math|f>|<point|-6.37346209816113|3.7397307844953>>|<text-at|<math|g>|<point|-5.82524143405212|1.50239780394232>>|<text-at|<math|v<rsub|l>>|<point|-4.94369294880275|3.06221060986903>>|<text-at|<math|v<rsub|k>>|<point|-7.6551627199365|2.48435639634872>>|<point|-7.47975922741103|3.22362415663448>>>|<label|touchface>Faces
    <math|f> and <math|g> having two vertices but no edges in common.>
  </float>. In Figures<nbsp><reference|blockjoin>, <reference|chordface>
  and<nbsp><reference|touchface>, the temporary edges added by the algorithm
  are drawn in green.

  This method of drawing planar graphs operates in
  <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> time. Nevertheless, it is
  quite fast for graphs up to 1000 vertices, usually producing results in
  less than a second. A drawback of this method is that it sometimes creates
  clusters of vertices which are very close to each other, resulting in a
  very high ratio of the area of the largest inner face to the area of the
  smallest inner face. However, if the result is not satisfactory, one can
  simply redraw the graph and repeat the process until a better layout is
  obtained. The planar embedding will in general be different each time if
  the graph is not triconnected.

  Another drawback of this method is that sparse planar graphs are sometimes
  drawn poorly.

  The following example shows that the above described improvement of the
  barycentric method handles non-triconnected graphs well.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(trail(1,2,3,4,5,6,7,8,9,10,1),trail(11,12,6,11,1,12))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 12 vertices and 15 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar)
    </input>
  </session>

  <center|<image|images/planar1.eps|35%|||>>

  Note that the inner diamond-like shape in the above drawing would end up
  flattened\Vmaking the two triangular faces invisible\Vif the input graph
  was not augmented. It is so because the vertices with labels 11 and 12 are
  \Pattracted\Q to each other (namely, the two large faces are \Pinflating\Q
  themselves to become convex), causing them to merge eventually.

  In the following example the input graph <math|G> is connected but not
  biconnected (it has two articulation points). It is obtained by removing a
  vertex from the Sierpi«ski triangle graph <math|S T<rsub|3><rsup|3>>. Note
  that the syntax mode is set to <samp|Xcas> in this example, so the first
  vertex label is zero.\ 

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=sierpinski_graph(3,3,triangle)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 15 vertices and 27 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=delete_vertex(G,3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 14 vertices and 23 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,planar,labels=false)
    </input>
  </session>

  <center|<image|images/planar2.eps|35%|||>>

  In the above example, several redraws were required to obtain a good planar
  embedding.

  <subsection|Circular graph drawings><label|circle-method>

  The drawing method selected by specifying the option <verbatim|circle=L> or
  <verbatim|convexhull=L> when calling <verbatim|draw_graph> on a
  triconnected graph <math|G<around*|(|V,E|)>>, where <math|L\<subset\>V> is
  a set of vertices in <math|G>, uses the following strategy. First,
  positions of the vertices from <math|L> are fixed so that they form a
  regular polygon on the unit circle. Other vertices, i.e.<nbsp>all vertices
  from <math|V\<setminus\>L>, are placed in origin. Then an iterative
  force-directed algorithm<nbsp><cite|plestenjak>, similar to <name|Tutte>'s
  barycentric method, is applied to obtain the final layout.

  This approach gives best results for symmetrical graphs such as generalized
  Petersen graphs. In addition, if the input graph is planar and
  triconnected, and the outer hull represents a face in the respective planar
  embedding, then the drawing will contain no edge crossings. There is a
  possibility, however, that some very short edges may cross each other as
  the number of force update iterations is limited.

  In the following example the Sierpi«ski graph <math|S<rsub|4><rsup|2>> is
  drawn using the above method. Note that the command lines below are
  executed in <samp|Xcas> mode.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=sierpinski_graph(2,4)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 16 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle=[0,1,4,5,7,13,15,14,11,10,8,2])
    </input>
  </session>

  <center|<image|images/sg24.eps|35%|||>>

  To draw a planar triconnected graph, one should pass one of its faces as
  the outer hull.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=truncate_graph(graph("frucht"))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 60 vertices and 90 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      purge(F):; is_planar(G,F)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<text|true>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle=rand(F),labels=false)
    </input>
  </session>

  <center|<image|images/tfrucht.eps|35%|||>>

  <section|Vertex positions>

  <subsection|Setting vertex positions><label|set-vertex-positions>

  The command <verbatim|set_vertex_positions><index|<verbatim|set_vertex_positions>>
  is used to assign custom coordinates to vertices of a graph to be used when
  drawing the graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|set_vertex_positions(G,L)>>>>>>

  <verbatim|set_vertex_positions> takes two arguments, a graph
  <math|G<around*|(|V,E|)>> and the list <math|L> of positions to be assigned
  to vertices in order of <verbatim|vertices(G)>. The positions may be
  complex numbers, lists of coordinates or points (geometrical objects
  created with the command <verbatim|point>). <verbatim|set_vertex_positions>
  returns the copy <math|G<rprime|'>> of <math|G> with the given layout
  stored in it.

  Any subsequent call to <verbatim|<hlink|draw_graph|#draw-graph>> with
  <math|G<rprime|'>> as an argument and without specifying the drawing style
  will result in displaying vertices at the stored coordinates. However, if a
  drawing style is specified, the stored layout is ignored (although it stays
  stored in <math|G<rprime|'>>).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=digraph([1,2,3,4,5],%{[1,2],[2,3],[3,4],[2,5]%})
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,circle)
    </input>
  </session>

  <center|<image|images/digraph1.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=set_vertex_positions(G,<code|[[0,0],[0.5,0],[1.0,0],[1.5,0],[0.5,1]]>)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/digraph2.eps|35%|||>>

  <subsection|Generating vertex positions>

  Vertex positions can be generated for a particular graph <math|G> by using
  the <verbatim|<hlink|draw_graph|#draw-graph>> command with the additional
  argument <math|P> which should be an unassigned identifier. After the
  layout is obtained, it will be stored to <math|P> as a list of positions
  (complex numbers for <abbr|2D> drawings or points for <abbr|3D> drawings)
  for each vertex in order of <verbatim|vertices(G)>.

  This feature combines well with the <verbatim|<hlink|set_vertex_positions|#set-vertex-positions>>
  command, as when one obtains the desired drawing of the graph <math|G> by
  calling <verbatim|draw_graph>, the layout coordinates can be easily stored
  to the graph for future reference. In particular, each subsequent call of
  <verbatim|draw_graph> with <math|G> as an argument will display the stored
  layout. The example below illustrates this property by setting a custom
  layout to the octahedral graph.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("octahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/oct2.eps|35%|||>>

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,P,spring):;
    </input>
  </session>

  Now <math|P> contains vertex coordinates, which can be permanently stored
  to <math|G>:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=set_vertex_positions(G,P)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G)
    </input>
  </session>

  <center|<image|images/oct1.eps|35%|||>>

  It should be noted that, after a particular layout is fixed, it stays valid
  when some edges or vertices are removed or when an edge is contracted. The
  stored layout becomes invalid only if a new vertex is added to the graph
  (unless its position is specified by <verbatim|<hlink|set_vertex_attribute|#vertex-attribute>>
  upon the creation) or if the <verbatim|position> attribute of an existing
  vertex is discarded.

  <section|Highlighting parts of graphs>

  <subsection|Highlighting vertices><label|highlight-vertices>

  The command <verbatim|highlight_vertex><index|<verbatim|highlight_vertex>>
  is used for changing color of one or more vertices in a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|highlight_vertex(G,v)>>>|<row|<cell|>|<cell|<verbatim|highlight_vertex(G,v,c)>>>|<row|<cell|>|<cell|<verbatim|highlight_vertex(G,[v1,v2,..,vk])>>>|<row|<cell|>|<cell|<verbatim|highlight_vertex(G,[v1,v2,..,vk],c)>>>|<row|<cell|>|<cell|<verbatim|highlight_vertex(G,[v1,v2,..,vk],[c1,c2,..,ck])>>>>>>

  <verbatim|highlight_vertex> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>>, a vertex <math|v\<in\>V> or a list of vertices
  <math|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|k>\<in\>V> and optionally the
  new color <math|c> or a list of colors <math|c<rsub|1>,c<rsub|2>,\<ldots\>,c<rsub|k>>
  for the selected vertices (the default color is green). It returns a
  modified copy of <math|G> in which the specified vertices are colored with
  the specified color.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph("dodecahedron")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 30 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L:=maximum_independent_set(G)
    <|unfolded-io>
      <\equation*>
        <around|[|2,4,6,12,13,10,16,19|]>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_vertex(G,L))
    </input>
  </session>

  <center|<image|images/dh1.eps|35%|||>>

  <subsection|Highlighting edges and trails><label|highlight-trail>

  To highlight an edge or a set of edges in a graph, use the
  <verbatim|highlight_edges><index|<verbatim|highlight_edges>> command. If
  the edges form a trail, it is usually more convenient to use the
  <verbatim|highlight_trail><index|<verbatim|highlight_trail>> command (see
  below).

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|highlight_edges(G,e)>>>|<row|<cell|>|<cell|<verbatim|highlight_edges(G,e,c)>>>|<row|<cell|>|<cell|<verbatim|highlight_edges(G,[e1,e2,..,ek])>>>|<row|<cell|>|<cell|<verbatim|highlight_edges(G,[e1,e2,..,ek],c)>>>|<row|<cell|>|<cell|<verbatim|highlight_edges(G,[e1,e2,..,ek],[c1,c2,..,ck])>>>|<row|<cell|>|<cell|<verbatim|highlight_trail(G,T)>>>|<row|<cell|>|<cell|<verbatim|highlight_trail(G,T,c)>>>|<row|<cell|>|<cell|<verbatim|highlight_trail(G,[T1,T2,..,Tk])>>>|<row|<cell|>|<cell|<verbatim|highlight_trail(G,[T1,T2,..,Tk],c)>>>|<row|<cell|>|<cell|<verbatim|highlight_trail(G,[T1,T2,..,Tk],[c1,c2,..,ck])>>>>>>

  <verbatim|highlight_edges> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>>, an edge <math|e\<in\>E> or a list of edges
  <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|k>\<in\>E> and optionally the
  new color <math|c> or a list of colors <math|c<rsub|1>,c<rsub|2>,\<ldots\>,c<rsub|k>>
  for the selected edges (the default color is red). It returns a modified
  copy of <math|G> in which the specified edges are colored with the
  specified color.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      M:=maximum_matching(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|5,4|]>,<around|[|6,11|]>,<around|[|3,8|]>,<around|[|7,12|]>,<around|[|9,13|]>,<around|[|10,15|]>,<around|[|14,19|]>,<around|[|16,17|]>|}>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,M))
    </input>
  </session>

  <center|<image|images/dh2.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=spanning_tree(G)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 20 vertices and 19 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,edges(S),magenta))
    </input>
  </session>

  <center|<image|images/dh3.eps|35%|||>>

  <verbatim|highlight_trail> takes two or three arguments: a graph
  <math|G<around*|(|V,E|)>>, a trail <math|T> or a list of trails
  <math|T<rsub|1>,T<rsub|2>,\<ldots\>,T<rsub|k>> and optionally the new color
  <math|c> or a list of colors <math|c<rsub|1>,c<rsub|2>,\<ldots\>,c<rsub|k>>.
  The command returns the copy of <math|G> in which edges between consecutive
  vertices in each of the given trails are highlighted with color <math|c>
  (by default red) or the trail <math|T<rsub|i>> is highlighted with color
  <math|c<rsub|i>> for <math|i=1,2,\<ldots\>,k>.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(G,[6,15,20,19,18,17,16,11,7,2,3,8,13,9,14,10]))
    </input>
  </session>

  <center|<image|images/dh4.eps|35%|||>>

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(G,shortest_path(G,1,[19,12]),[green,magenta]))
    </input>
  </session>

  <center|<image|images/hlt.eps|35%|||>>

  <subsection|Highlighting subgraphs><label|highlight-subgraph>

  The command <verbatim|highlight_subgraph><index|<verbatim|highlight_subgraph>>
  is used for highlighting subgraph(s) of a graph.

  <tabular|<tformat|<cwith|1|1|1|1|cell-lsep|0>|<table|<row|<cell|Syntax:>|<cell|<verbatim|highlight_subgraph(G,S,[weights])>>>|<row|<cell|>|<cell|<verbatim|highlight_subgraph(G,S,c1,c2,[weights])>>>|<row|<cell|>|<cell|<verbatim|highlight_subgraph(G,[S1,S2,..,Sk])>>>|<row|<cell|>|<cell|<verbatim|highlight_subgraph(G,[S1,S2,..,Sk],c1,c2)>>>>>>

  <verbatim|highlight_subgraph> takes two or four mandatory arguments: a
  graph <math|G<around*|(|V,E|)>>, a subgraph
  <math|S<around*|(|V<rprime|'>,E<rprime|'>|)>> of <math|G> or a list of
  subgraphs <math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|k>> in <math|G> and
  optionally the new colors <math|c<rsub|1>,c<rsub|2>> for the edges and
  vertices of the selected subgraph(s), respectively. It returns a modified
  copy of <math|G> with the selected subgraph(s) colored as specified. If
  colors are not given, red and green are used, respectively.

  The option <verbatim|weights> may be passed as an additional argument if
  <math|G> and <math|S> are weighted graphs. In that case, the weights of
  edges in <math|E<rprime|'>\<subset\>E> in <math|G> are overwritten with
  those defined in <math|S> for the same edges.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3],[3,1],[3,4],[4,5],[5,6],[6,4]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      A:=articulation_points(G)
    <|unfolded-io>
      <\equation*>
        <around|[|3,4|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      B:=biconnected_components(G)
    <|unfolded-io>
      <\equation*>
        <around|[|<around|[|4,6,5|]>,<around|[|3,4|]>,<around|[|1,3,2|]>|]>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=highlight_vertex(G,A,magenta)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H)
    </input>
  </session>

  <center|<image|images/sg1.eps|35%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=induced_subgraph(G,B[0])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      H:=highlight_subgraph(G,S)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(H,spring)
    </input>
  </session>

  <center|<image|images/sg2.eps|35%|||>>

  <\bibliography|bib|tm-plain|graphtheory>
    <\bib-list|65>
      <bibitem*|1><label|bib-afzal>Shehzad Afzal<localize| and >Clemens
      Brand. <newblock>Recognizing triangulated Cartesian graph products.
      <newblock><with|font-shape|italic|Discrete Mathematics>, 312:188\U193,
      2012.<newblock>

      <bibitem*|2><label|bib-alonso>L.<nbsp>Alonso<localize| and
      >R.<nbsp>Schott. <newblock>Random Unlabelled Rooted Trees Revisited.
      <newblock><localize|In ><with|font-shape|italic|Proc.<nbsp>Int.<nbsp>Conf.<nbsp>on
      Computing and Information 1994>, <localize|pages >1352\U1367.<newblock>

      <bibitem*|3><label|bib-andova>Vesna Andova, Franti²ek Kardo²<localize|,
      and >Riste ’krekovski. <newblock>Mathematical aspects of fullerenes.
      <newblock><with|font-shape|italic|Ars Mathematica Contemporanea>,
      11:353\U379, 2016.<newblock>

      <bibitem*|4><label|bib-bagatelj>Vladimir Bagatelj<localize| and >Ulrik
      Brandes. <newblock>Efficient generation of large random networks.
      <newblock><with|font-shape|italic|Physical Review E>, 71:036113,
      2005.<newblock>

      <bibitem*|5><label|bib-bayati>Mohsen Bayati, Jeong<nbsp>Han
      Kim<localize|, and >Amin Saberi. <newblock>A Sequential Algorithm for
      Generating Random Graphs. <newblock><with|font-shape|italic|Algorithmica>,
      58:860\U910, 2010.<newblock>

      <bibitem*|6><label|bib-bellman>R.<nbsp>Bellman. <newblock>On a routing
      problem. <newblock><with|font-shape|italic|Quarterly of Applied
      Mathematics>, 16:87\U90, 1958.<newblock>

      <bibitem*|7><label|bib-biggs>Norman Biggs.
      <newblock><with|font-shape|italic|Algebraic graph theory>.
      <newblock>Cambridge University Press, Second<localize| edition>,
      1993.<newblock>

      <bibitem*|8><label|bib-blanusa>Danilo Blanu²a. <newblock>Problem
      £etiriju boja. <newblock><with|font-shape|italic|Glasnik
      Mat.-Fiz.<nbsp>Astr.<nbsp>Ser.<nbsp>II>, 1:31\U32, 1946.<newblock>

      <bibitem*|9><label|bib-bollobas>Béla Bollobás.
      <newblock><with|font-shape|italic|Modern Graph Theory>.
      <newblock>Graduate Texts in Mathematics. Springer, Corrected<localize|
      edition>, 2002.<newblock>

      <bibitem*|10><label|bib-boot>Coen Boot. <newblock>Algorithms for
      Determining the Clustering Coefficient in Large Graphs.
      <newblock>Bachelor's thesis, Faculty of Science, Utrecht University,
      2016.<newblock>

      <bibitem*|11><label|bib-brelaz>Daniel Brélaz. <newblock>New Methods to
      Color the Vertices of a Graph. <newblock><with|font-shape|italic|Communications
      of the ACM>, 22:251\U256, 1979.<newblock>

      <bibitem*|12><label|bib-buchheim>Cristoph Buchheim, Michael
      Jünger<localize|, and >Sebastian Leipert. <newblock>Improving Walker's
      Algorithm to Run in Linear Time. <newblock><localize|In
      >M.<nbsp>T.<nbsp>Goodrich<localize| and
      >S.<nbsp>G.<nbsp>Kobourov<localize|, editors>,
      <with|font-shape|italic|Graph Drawing 2002, Lecture Notes in Computer
      Science vol 2528>, <localize|pages >344\U353. Springer-Verlag Berlin
      Heidelberg, 2002.<newblock>

      <bibitem*|13><label|bib-christofides>Nicos Christofides.
      <newblock>Worst-case analysis of a new heuristic for the traveling
      salesman problem. <newblock>Report 388, Graduate School of Industrial
      Administration, 1976.<newblock>

      <bibitem*|14><label|bib-cook>William<nbsp>J.<nbsp>Cook.
      <newblock><with|font-shape|italic|In Pursuit of the Traveling Salesman:
      Mathematics at the Limits of Computation>. <newblock>Princeton
      University Press, 2012.<newblock>

      <bibitem*|15><label|bib-melissa>Melissa DeLeon. <newblock>A Study of
      Sufficient Conditions for Hamiltonian Cycles.
      <newblock><with|font-shape|italic|Rose-Hulman Undergraduate Mathematics
      Journal>, 1, Article 6, 2000. <newblock><slink|https://scholar.rose-hulman.edu/rhumj/vol1/iss1/6>.<newblock>

      <bibitem*|16><label|bib-diaz>Isabel<nbsp>M.<nbsp>Díaz<localize| and
      >Paula Zabala. <newblock>A Branch-and-Cut Algorithm for Graph Coloring.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      154:826\U847, 2006.<newblock>

      <bibitem*|17><label|bib-diestel>Reinhard Diestel.
      <newblock><with|font-shape|italic|Graph Theory>.
      <newblock>Springer-Verlag, New York, 1997.<newblock>

      <bibitem*|18><label|bib-dijkstra>Edsger<nbsp>W.<nbsp>Dijkstra.
      <newblock>A note on Two Problems in Connexion with Graphs.
      <newblock><with|font-shape|italic|Numerische Mathematik>, 1:269\U271,
      1959.<newblock>

      <bibitem*|19><label|bib-edmonds>Jack Edmonds. <newblock>Paths, Trees,
      and Flowers. <newblock><localize|In >Gessel I.<localize| and
      >GC.<nbsp>Rota<localize|, editors>, <with|font-shape|italic|Classic
      Papers in Combinatorics>, <localize|pages >361\U379. Birkhäuser Boston,
      2009. Modern Birkhäuser Classics.<newblock>

      <bibitem*|20><label|bib-edmonds-karp>Jack Edmonds<localize| and
      >Richard<nbsp>M.<nbsp>Karp. <newblock>Theoretical improvements in
      algorithmic efficiency for network flow problems.
      <newblock><with|font-shape|italic|Journal of the ACM>, 19:248\U264,
      1972.<newblock>

      <bibitem*|21><label|bib-esfahanian>Abdol<nbsp>H.<nbsp>Esfahanian<localize|
      and >S.<nbsp>Louis Hakimi. <newblock>On computing the connectivities of
      graphs and digraphs. <newblock><with|font-shape|italic|Networks>,
      14:355\U366, 1984.<newblock>

      <bibitem*|22><label|bib-even-algorithms>Shimon Even.
      <newblock><with|font-shape|italic|Graph Algorithms>. <newblock>Computer
      software engineering series. Computer Science Press, 1979.<newblock>

      <bibitem*|23><label|bib-floyd>Robert<nbsp>W.<nbsp>Floyd.
      <newblock>Algorithm 97: Shortest path.
      <newblock><with|font-shape|italic|Communications of the ACM>, 5:345,
      1962.<newblock>

      <bibitem*|24><label|bib-ford>L.<nbsp>R.<nbsp>Ford.
      <newblock><with|font-shape|italic|Network flow theory>. <newblock>Rand
      Corporation, 1956.<newblock>

      <bibitem*|25><label|bib-fruchterman>T.<nbsp>M.<nbsp>J.<nbsp>Fruchterman<localize|
      and >E.<nbsp>M.<nbsp>Reingold. <newblock>Graph Drawing by
      Force-Directed Placement. <newblock><with|font-shape|italic|Software:
      Practice and Experience>, 21:1129\U1164, 1991.<newblock>

      <bibitem*|26><label|bib-gibbons>Alan Gibbons.
      <newblock><with|font-shape|italic|Algorithmic graph theory>.
      <newblock>Cambridge University Press, 1985.<newblock>

      <bibitem*|27><label|bib-godsil>Chris Godsil<localize| and
      >Gordon<nbsp>F.<nbsp>Royle. <newblock><with|font-shape|italic|Algebraic
      graph theory>. <newblock>Graduate Texts in Mathematics. Springer,
      First<localize| edition>, 2001.<newblock>

      <bibitem*|28><label|bib-goldfarb>Donald Goldfarb<localize| and
      >Michael<nbsp>D.<nbsp>Grigoriadis. <newblock>A computational comparison
      of the dinic and network simplex methods for maximum flow.
      <newblock><with|font-shape|italic|Annals of Operations Research>,
      13:81\U123, 1988.<newblock>

      <bibitem*|29><label|bib-haggard>Gary Haggard,
      David<nbsp>J.<nbsp>Pearce<localize|, and >Gordon Royle.
      <newblock>Computing Tutte Polynomials.
      <newblock><with|font-shape|italic|ACM Transactions on Mathematical
      Software>, 37, 2010. <newblock>Article No.<nbsp>24.<newblock>

      <bibitem*|30><label|bib-haggard2>Gary Haggard,
      David<nbsp>J.<nbsp>Pearce<localize|, and >Gordon Royle.
      <newblock>Edge-Selection Heuristics for Computing Tutte Polynomials.
      <newblock><with|font-shape|italic|Chicago Journal of Theoretical
      Computer Science>, 2010. <newblock>Article 6.<newblock>

      <bibitem*|31><label|bib-hakimi>S.<nbsp>L.<nbsp>Hakimi. <newblock>On
      realizability of a set of integers as degrees of the vertices of a
      linear graph. I. <newblock><with|font-shape|italic|Journal of the
      Society for Industrial and Applied Mathematics>, 10:496\U506,
      1962.<newblock>

      <bibitem*|32><label|bib-helsgaun>Keld Helsgaun. <newblock>General
      <math|k>-opt submoves for the Lin--Kernighan TSP heuristic.
      <newblock><with|font-shape|italic|Math.<nbsp>Prog.<nbsp>Comp.>,
      1:119\U163, 2009.<newblock>

      <bibitem*|33><label|bib-hierholzer>Carl Hierholzer. <newblock>Ueber die
      möglichkeit, einen Linienzug ohne Wiederholung und ohne Unterbrechung
      zu umfahren. <newblock><with|font-shape|italic|Mathematische Annalen>,
      6:30\U32, 1873.<newblock>

      <bibitem*|34><label|bib-hinz>Andreas<nbsp>M.<nbsp>Hinz, Sandi
      Klavºar<localize|, and >Sara<nbsp>S.<nbsp>Zemlji£. <newblock>A survey
      and classification of Sierpi«ski-type graphs.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      217:565\U600, 2017.<newblock>

      <bibitem*|35><label|bib-hopcroft>John<nbsp>E.<nbsp>Hopcroft<localize|
      and >Richard<nbsp>M.<nbsp>Karp. <newblock>An <math|n<rsup|5/2>>
      algorithm for maximum matchings in bipartite graphs.
      <newblock><with|font-shape|italic|SIAM Journal on Computing>,
      2:225\U231, 1973.<newblock>

      <bibitem*|36><label|bib-hu>Yifan Hu. <newblock>Efficient and High
      Quality Force-Directed Graph Drawing.
      <newblock><with|font-shape|italic|Mathematica Journal>, 10:37\U71,
      2005.<newblock>

      <bibitem*|37><label|bib-hu2>Yifan Hu<localize| and >Jennifer Scott.
      <newblock>A Multilevel Algorithm for Wavefront Reduction.
      <newblock><with|font-shape|italic|SIAM Journal on Scientific
      Computing>, 23:1352\U1375, 2001.<newblock>

      <bibitem*|38><label|bib-kahn>Arthur<nbsp>B.<nbsp>Kahn.
      <newblock>Topological sorting of large networks.
      <newblock><with|font-shape|italic|Communications of the ACM>,
      5:558\U562, 1962.<newblock>

      <bibitem*|39><label|bib-latapy>Matthieu Latapy. <newblock>Main-memory
      triangle computations for very large (sparse (power-law)) graphs.
      <newblock><with|font-shape|italic|Theor.<nbsp>Comput.<nbsp>Sci.>,
      407:458\U473, 2008.<newblock>

      <bibitem*|40><label|bib-mckay>B.<nbsp>D.<nbsp>McKay<localize| and
      >A.<nbsp>Piperno. <newblock>Practical Graph Isomorphism, II.
      <newblock><with|font-shape|italic|J.<nbsp>Symbolic Computation>,
      60:94\U112, 2013.<newblock>

      <bibitem*|41><label|bib-monagan>Michael Monagan. <newblock>A new edge
      selection heuristic for computing Tutte polynomials.
      <newblock><localize|In ><with|font-shape|italic|Proceedings of FPSAC
      2012>, <localize|pages >839\U850.<newblock>

      <bibitem*|42><label|bib-myrwold>Wendy Myrwold<localize| and >Willian
      Kocay. <newblock>Errors in graph embedding algorithms.
      <newblock><with|font-shape|italic|Journal of Computer and System
      Sciences>, 77:430\U438, 2011.<newblock>

      <bibitem*|43><label|bib-newman>M.<nbsp>E.<nbsp>Newman,
      D.<nbsp>J.<nbsp>Watts<localize|, and >S.<nbsp>H.<nbsp>Strogatz.
      <newblock>Random graph models of social networks.
      <newblock><with|font-shape|italic|Proc Natl Acad Sci USA>,
      99:2566\U2572, 2002.<newblock>

      <bibitem*|44><label|bib-nijenhuis>Albert Nijenhuis<localize| and
      >Herbert<nbsp>S.<nbsp>Wilf. <newblock><with|font-shape|italic|Combinatorial
      Algorithms>. <newblock>Computer Science and Applied Mathematics.
      Academic Press, Second<localize| edition>, 1978.<newblock>

      <bibitem*|45><label|bib-ostergard>Patric<nbsp>R.<nbsp>J.<nbsp>Östergård.
      <newblock>A fast algorithm for the maximum clique problem.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      120:197\U207, 2002.<newblock>

      <bibitem*|46><label|bib-otter>Richard Otter. <newblock>The Number of
      Trees. <newblock><with|font-shape|italic|The Annals of Mathematics, 2nd
      Ser.>, 49:583\U599, 1948.<newblock>

      <bibitem*|47><label|bib-padberg>Manfred Padberg<localize| and >Giovanni
      Rinaldi. <newblock>A Branch-and-Cut Algorithm for the Resolution of
      Large-Scale Symmetric Traveling Salesman Problems.
      <newblock><with|font-shape|italic|SIAM Review>, 33:60\U100,
      1991.<newblock>

      <bibitem*|48><label|bib-cpap>Charalampos Papamanthou<localize| and
      >Ioannis<nbsp>G.<nbsp>Tollis. <newblock>Algorithms for computing a
      parametrized st-orientation. <newblock><with|font-shape|italic|Theoretical
      Computer Science>, 408:224\U240, 2008.<newblock>

      <bibitem*|49><label|bib-pferschy>Ulrich Pferschy<localize| and
      >Rostislav Stan¥k. <newblock>Generating subtour elimination constraints
      for the TSP from pure integer solutions.
      <newblock><with|font-shape|italic|Central European Journal of
      Operations Research>, 25:231\U260, 2017.<newblock>

      <bibitem*|50><label|bib-plestenjak>Bor Plestenjak. <newblock>An
      Algorithm for Drawing Planar Graphs.
      <newblock><with|font-shape|italic|Software: Practice and Experience>,
      29:973\U984, 1999.<newblock>

      <bibitem*|51><label|bib-schank2>T.<nbsp>Schank<localize| and
      >D.<nbsp>Wagner. <newblock>Finding, Counting and Listing All Triangles
      in Large Graphs, an Experimental Study. <newblock><localize|In
      >S.<nbsp>E.<nbsp>Nikoletseas<localize|, editor>,
      <with|font-shape|italic|Experimental and Efficient Algorithms.<nbsp>WEA
      2005.<nbsp>Lecture Notes in Computer Science>, <localize|volume> 3503,
      <localize|pages >606\U609. Springer, Berlin, Heidelberg,
      2005.<newblock>

      <bibitem*|52><label|bib-schank>Thomas Schank<localize| and >Dorothea
      Wagner. <newblock>Approximating Clustering Coefficient and
      Transitivity. <newblock><with|font-shape|italic|Journal of Graph
      Algorithms and Applications>, 9:265\U275, 2005.<newblock>

      <bibitem*|53><label|bib-steger>Angelika Steger<localize| and
      >Nicholas<nbsp>C.<nbsp>Wormald. <newblock>Generating random regular
      graphs quickly. <newblock><with|font-shape|italic|Combinatorics
      Probability and Computing>, 8:377\U396, 1999.<newblock>

      <bibitem*|54><label|bib-tarjan72>R.<nbsp>E.<nbsp>Tarjan.
      <newblock>Depth-First Search and Linear Graph Algorithms.
      <newblock><with|font-shape|italic|SIAM Journal on Comp.>, 1:146\U160,
      1972.<newblock>

      <bibitem*|55><label|bib-tarjan-bridges>R.<nbsp>E.<nbsp>Tarjan.
      <newblock>A note on finding the bridges of a graph.
      <newblock><with|font-shape|italic|Information Processing Letters>,
      2:160\U161, 1974.<newblock>

      <bibitem*|56><label|bib-lca>R.<nbsp>E.<nbsp>Tarjan.
      <newblock>Applications of path compression on balanced trees.
      <newblock><with|font-shape|italic|Journal of the ACM>, 26:690\U715,
      1979.<newblock>

      <bibitem*|57><label|bib-tarjan86>R.<nbsp>E.<nbsp>Tarjan. <newblock>Two
      streamlined depth-first search algorithms.
      <newblock><with|font-shape|italic|Fundamenta Informaticae>, 9:85\U94,
      1986.<newblock>

      <bibitem*|58><label|bib-krishnaiyan>K.<nbsp>Thulasiraman,
      S.<nbsp>Arumugam, A.<nbsp>Brandstädt<localize|, and
      >T.<nbsp>Nishizeki<localize|, editors>.
      <newblock><with|font-shape|italic|Handbook of Graph Theory,
      Combinatorial Optimization, and Algorithms>. <newblock>CRC Press,
      2016.<newblock>

      <bibitem*|59><label|bib-tomita>Etsuji Tomita, Akira Tanaka<localize|,
      and >Haruhisa Takahashi. <newblock>The worst-case time complexity for
      generating all maximal cliques and computational experiments.
      <newblock><with|font-shape|italic|Theoretical Computer Science>,
      363:28\U42, 2006.<newblock>

      <bibitem*|60><label|bib-tutte>W.<nbsp>T.<nbsp>Tutte. <newblock>How to
      draw a graph. <newblock><with|font-shape|italic|Proceedings of the
      London Mathematical Society>, s3-13:743\U767, 1963.<newblock>

      <bibitem*|61><label|bib-walker>John<nbsp>Q.<nbsp>Walker II. <newblock>A
      nodepositioning algorithm for general trees.
      <newblock><with|font-shape|italic|Software: Practice and Experience>,
      20:685\U705, 1990.<newblock>

      <bibitem*|62><label|bib-wasserman>Stanley Wasserman<localize| and
      >Katherine Faust. <newblock><with|font-shape|italic|Social Network
      Analysis: Methods and Applications>. <newblock>Cambridge University
      Press, 1994.<newblock>

      <bibitem*|63><label|bib-welch>E.<nbsp>Welch<localize| and
      >S.<nbsp>Kobourov. <newblock>Measuring Symmetry in Drawings of Graphs.
      <newblock><with|font-shape|italic|Computer Graphics Forum>,
      36:341\U351, 2017.<newblock>

      <bibitem*|64><label|bib-west>Douglas<nbsp>B.<nbsp>West.
      <newblock><with|font-shape|italic|Introduction to Graph Theory>.
      <newblock>Pearson Education, 2002.<newblock>

      <bibitem*|65><label|bib-wilf>Herbert<nbsp>S.<nbsp>Wilf. <newblock>The
      Uniform Selection of Free Trees. <newblock><with|font-shape|italic|Journal
      of Algorithms>, 2:204\U207, 1981.<newblock>
    </bib-list>
  </bibliography>

  <\the-index|idx>
    <index-1|<with|font-family|tt|add_arc>|<pageref|auto-138>>

    <index-1|<with|font-family|tt|add_edge>|<pageref|auto-137>>

    <index-1|<with|font-family|tt|add_vertex>|<pageref|auto-133>>

    <index-1|<with|font-family|tt|language|verbatim|adjacency_matrix>|<pageref|auto-212>>

    <index-1|<with|font-family|tt|language|verbatim|allpairs_distance>|<pageref|auto-289>>

    <index-1|<with|font-family|tt|language|verbatim|antiprism_graph>|<pageref|auto-51>>

    <index-1|<with|font-family|tt|language|verbatim|arrivals>|<pageref|auto-203>>

    <index-1|<with|font-family|tt|language|verbatim|articulation_points>|<pageref|auto-256>>

    <index-1|<with|font-family|tt|language|verbatim|assign_edge_weights>|<pageref|auto-124>>

    <index-1|<with|font-family|tt|language|verbatim|bellman_ford>|<pageref|auto-354>>

    <index-1|<with|font-family|tt|language|verbatim|biconnected_components>|<pageref|auto-250>>

    <index-1|<with|font-family|tt|language|verbatim|bipartite_matching>|<pageref|auto-306>>

    <index-1|<with|font-family|tt|language|verbatim|canonical_labeling>|<pageref|auto-231>>

    <index-1|<with|font-family|tt|cartesian_product>|<pageref|auto-94>>

    <index-1|<with|font-family|tt|language|verbatim|chromatic_index>|<pageref|auto-342>>

    <index-1|<with|font-family|tt|language|verbatim|chromatic_number>|<pageref|auto-333>>

    <index-1|<with|font-family|tt|language|verbatim|chromatic_polynomial>|<pageref|auto-238>>

    <index-1|<with|font-family|tt|clique_cover>|<pageref|auto-316>>

    <index-1|<with|font-family|tt|language|verbatim|clique_cover_number>|<pageref|auto-318>>

    <index-1|<with|font-family|tt|language|verbatim|clique_number>|<pageref|auto-314>>

    <index-1|<with|font-family|tt|clique_stats>|<pageref|auto-311>>

    <index-1|<with|font-family|tt|language|verbatim|clustering_coefficient>|<pageref|auto-323>>

    <index-1|<with|font-family|tt|language|verbatim|complete_binary_tree>|<pageref|auto-26>>

    <index-1|<with|font-family|tt|language|verbatim|complete_graph>|<pageref|auto-24>>

    <index-1|<with|font-family|tt|language|verbatim|complete_kary_tree>|<pageref|auto-27>>

    <index-1|<with|font-family|tt|language|verbatim|connected_components>|<pageref|auto-249>>

    <index-1|<with|font-family|tt|contract_edge>|<pageref|auto-145>>

    <index-1|<with|font-family|tt|language|verbatim|cycle_basis>|<pageref|auto-77>>

    <index-1|<with|font-family|tt|language|verbatim|cycle_graph>|<pageref|auto-16>>

    <index-1|<with|font-family|tt|language|verbatim|degree_sequence>|<pageref|auto-191>>

    <index-1|<with|font-family|tt|delete_arc>|<pageref|auto-140>>

    <index-1|<with|font-family|tt|delete_edge>|<pageref|auto-139>>

    <index-1|<with|font-family|tt|delete_vertex>|<pageref|auto-134>>

    <index-1|<with|font-family|tt|language|verbatim|departures>|<pageref|auto-202>>

    <index-1|<with|font-family|tt|language|verbatim|digraph>|<pageref|auto-7>>

    <index-1|<with|font-family|tt|dijkstra>|<pageref|auto-353>>

    <index-1|<with|font-family|tt|discard_edge_attribute>|<pageref|auto-163>>

    <index-1|<with|font-family|tt|discard_graph_attribute>|<pageref|auto-153>>

    <index-1|<with|font-family|tt|discard_vertex_attribute>|<pageref|auto-158>>

    <index-1|<with|font-family|tt|disjoint_union>|<pageref|auto-88>>

    <index-1|<with|font-family|tt|draw_graph>|<pageref|auto-366>>

    <index-1|<with|font-family|tt|language|verbatim|edge_connectivity>|<pageref|auto-261>>

    <index-1|<with|font-family|tt|language|verbatim|edges>|<pageref|auto-182>>

    <index-1|<with|font-family|tt|export_graph>|<pageref|auto-171>>

    <index-1|<with|font-family|tt|language|verbatim|flow_polynomial>|<pageref|auto-240>>

    <index-1|<with|font-family|tt|language|verbatim|fundamental_cycle>|<pageref|auto-76>>

    <index-1|<with|font-family|tt|get_edge_attribute>|<pageref|auto-161>>

    <index-1|<with|font-family|tt|get_edge_weight>|<pageref|auto-142>>

    <index-1|<with|font-family|tt|get_graph_attribute>|<pageref|auto-151>>

    <index-1|<with|font-family|tt|get_vertex_attribute>|<pageref|auto-156>>

    <index-1|<with|font-family|tt|language|verbatim|girth>|<pageref|auto-292>>

    <index-1|<with|font-family|tt|language|verbatim|graph>|<pageref|auto-5>>

    <index-1|<with|font-family|tt|language|verbatim|graph_automorphisms>|<pageref|auto-233>>

    <index-1|<with|font-family|tt|language|verbatim|graph_charpoly>|<pageref|auto-220>>

    <index-1|<with|font-family|tt|graph_complement>|<pageref|auto-80>>

    <index-1|<with|font-family|tt|language|verbatim|graph_equal>|<pageref|auto-186>>

    <index-1|<with|font-family|tt|graph_join>|<pageref|auto-90>>

    <index-1|<with|font-family|tt|graph_power>|<pageref|auto-92>>

    <index-1|<with|font-family|tt|language|verbatim|graph_rank>|<pageref|auto-254>>

    <index-1|<with|font-family|tt|language|verbatim|graph_spectrum>|<pageref|auto-222>>

    <index-1|<with|font-family|tt|graph_union>|<pageref|auto-86>>

    <index-1|<with|font-family|tt|language|verbatim|graph_vertices>|<pageref|auto-181>>

    <index-1|<with|font-family|tt|greedy_color>|<pageref|auto-328>>

    <index-1|<with|font-family|tt|language|verbatim|grid_graph>|<pageref|auto-53>>

    <index-1|<with|font-family|tt|language|verbatim|has_arc>|<pageref|auto-200>>

    <index-1|<with|font-family|tt|language|verbatim|has_edge>|<pageref|auto-199>>

    <index-1|<with|font-family|tt|language|verbatim|highlight_edges>|<pageref|auto-384>>

    <index-1|<with|font-family|tt|language|verbatim|highlight_subgraph>|<pageref|auto-387>>

    <index-1|<with|font-family|tt|language|verbatim|highlight_trail>|<pageref|auto-385>>

    <index-1|<with|font-family|tt|language|verbatim|highlight_vertex>|<pageref|auto-382>>

    <index-1|<with|font-family|tt|language|verbatim|hypercube_graph>|<pageref|auto-41>>

    <index-1|<with|font-family|tt|language|verbatim|import_graph>|<pageref|auto-167>>

    <index-1|<with|font-family|tt|language|verbatim|incidence_matrix>|<pageref|auto-216>>

    <index-1|<with|font-family|tt|language|verbatim|incident_edges>|<pageref|auto-209>>

    <index-1|<with|font-family|tt|induced_subgraph>|<pageref|auto-72>>

    <index-1|<with|font-family|tt|language|verbatim|interval_graph>|<pageref|auto-35>>

    <index-1|<with|font-family|tt|language|verbatim|is_acyclic>|<pageref|auto-296>>

    <index-1|<with|font-family|tt|language|verbatim|is_arborescence>|<pageref|auto-277>>

    <index-1|<with|font-family|tt|language|verbatim|is_biconnected>|<pageref|auto-246>>

    <index-1|<with|font-family|tt|language|verbatim|is_bipartite>|<pageref|auto-207>>

    <index-1|<with|font-family|tt|is_clique>|<pageref|auto-309>>

    <index-1|<with|font-family|tt|language|verbatim|is_connected>|<pageref|auto-245>>

    <index-1|<with|font-family|tt|language|verbatim|is_cut_set>|<pageref|auto-263>>

    <index-1|<with|font-family|tt|language|verbatim|is_directed>|<pageref|auto-177>>

    <index-1|<with|font-family|tt|language|verbatim|is_eulerian>|<pageref|auto-346>>

    <index-1|<with|font-family|tt|language|verbatim|is_forest>|<pageref|auto-271>>

    <index-1|<with|font-family|tt|language|verbatim|is_graphic_sequence>|<pageref|auto-32>>

    <index-1|<with|font-family|tt|language|verbatim|is_hamiltonian>|<pageref|auto-348>>

    <index-1|<with|font-family|tt|language|verbatim|is_integer_graph>|<pageref|auto-226>>

    <index-1|<with|font-family|tt|language|verbatim|is_isomorphic>|<pageref|auto-229>>

    <index-1|<with|font-family|tt|language|verbatim|is_network>|<pageref|auto-280>>

    <index-1|<with|font-family|tt|language|verbatim|is_planar>|<pageref|auto-102>>

    <index-1|<with|font-family|tt|language|verbatim|is_regular>|<pageref|auto-195>>

    <index-1|<with|font-family|tt|language|verbatim|is_strongly_connected>|<pageref|auto-259>>

    <index-1|<with|font-family|tt|language|verbatim|is_strongly_regular>|<pageref|auto-197>>

    <index-1|<with|font-family|tt|language|verbatim|is_tournament>|<pageref|auto-205>>

    <index-1|<with|font-family|tt|language|verbatim|is_tree>|<pageref|auto-269>>

    <index-1|<with|font-family|tt|language|verbatim|is_triangle_free>|<pageref|auto-321>>

    <index-1|<with|font-family|tt|language|verbatim|is_triconnected>|<pageref|auto-247>>

    <index-1|<with|font-family|tt|language|verbatim|is_two_edge_connected>|<pageref|auto-265>>

    <index-1|<with|font-family|tt|language|verbatim|is_vertex_colorable>|<pageref|auto-337>>

    <index-1|<with|font-family|tt|language|verbatim|is_weighted>|<pageref|auto-178>>

    <index-1|<with|font-family|tt|isomorphic_copy>|<pageref|auto-63>>

    <index-1|<with|font-family|tt|language|verbatim|kneser_graph>|<pageref|auto-37>>

    <index-1|<with|font-family|tt|language|verbatim|laplacian_matrix>|<pageref|auto-214>>

    <index-1|<with|font-family|tt|language|verbatim|lcf_graph>|<pageref|auto-60>>

    <index-1|<with|font-family|tt|language|verbatim|line_graph>|<pageref|auto-99>>

    <index-1|<with|font-family|tt|list_edge_attributes>|<pageref|auto-162>>

    <index-1|<with|font-family|tt|list_graph_attributes>|<pageref|auto-152>>

    <index-1|<with|font-family|tt|list_vertex_attributes>|<pageref|auto-157>>

    <index-1|<with|font-family|tt|language|verbatim|lowest_common_ancestor>|<pageref|auto-275>>

    <index-1|<with|font-family|tt|make_directed>|<pageref|auto-128>>

    <index-1|<with|font-family|tt|make_weighted>|<pageref|auto-130>>

    <index-1|<with|font-family|tt|language|verbatim|maxflow>|<pageref|auto-282>>

    <index-1|<with|font-family|tt|maximum_clique>|<pageref|auto-313>>

    <index-1|<with|font-family|tt|language|verbatim|maximum_degree>|<pageref|auto-193>>

    <index-1|<with|font-family|tt|language|verbatim|maximum_matching>|<pageref|auto-304>>

    <index-1|<with|font-family|tt|language|verbatim|minimal_edge_coloring>|<pageref|auto-340>>

    <index-1|<with|font-family|tt|minimal_spanning_tree>|<pageref|auto-361>>

    <index-1|<with|font-family|tt|minimal_vertex_coloring>|<pageref|auto-331>>

    <index-1|<with|font-family|tt|language|verbatim|minimum_cut>|<pageref|auto-284>>

    <index-1|<with|font-family|tt|language|verbatim|minimum_degree>|<pageref|auto-192>>

    <index-1|<with|font-family|tt|language|verbatim|mycielski>|<pageref|auto-335>>

    <index-1|<with|font-family|tt|language|verbatim|neighbors>|<pageref|auto-201>>

    <index-1|<with|font-family|tt|language|verbatim|network_transitivity>|<pageref|auto-325>>

    <index-1|<with|font-family|tt|language|verbatim|number_of_edges>|<pageref|auto-184>>

    <index-1|<with|font-family|tt|language|verbatim|number_of_spanning_trees>|<pageref|auto-363>>

    <index-1|<with|font-family|tt|language|verbatim|number_of_vertices>|<pageref|auto-183>>

    <index-1|<with|font-family|tt|language|verbatim|odd_girth>|<pageref|auto-293>>

    <index-1|<with|font-family|tt|language|verbatim|odd_graph>|<pageref|auto-38>>

    <index-1|<with|font-family|tt|language|verbatim|path_graph>|<pageref|auto-18>>

    <index-1|<with|font-family|tt|permute_vertices>|<pageref|auto-65>>

    <index-1|<with|font-family|tt|language|verbatim|petersen_graph>|<pageref|auto-58>>

    <index-1|<with|font-family|tt|language|verbatim|plane_dual>|<pageref|auto-101>>

    <index-1|<with|font-family|tt|language|verbatim|prism_graph>|<pageref|auto-49>>

    <index-1|<with|font-family|tt|language|verbatim|random_bipartite_graph>|<pageref|auto-110>>

    <index-1|<with|font-family|tt|language|verbatim|random_digraph>|<pageref|auto-108>>

    <index-1|<with|font-family|tt|language|verbatim|random_graph>|<pageref|auto-107>>

    <index-1|<with|font-family|tt|language|verbatim|random_network>|<pageref|auto-122>>

    <index-1|<with|font-family|tt|language|verbatim|random_planar_graph>|<pageref|auto-114>>

    <index-1|<with|font-family|tt|language|verbatim|random_regular_graph>|<pageref|auto-118>>

    <index-1|<with|font-family|tt|language|verbatim|random_sequence_graph>|<pageref|auto-116>>

    <index-1|<with|font-family|tt|language|verbatim|random_tournament>|<pageref|auto-120>>

    <index-1|<with|font-family|tt|language|verbatim|random_tree>|<pageref|auto-112>>

    <index-1|<with|font-family|tt|relabel_vertices>|<pageref|auto-67>>

    <index-1|<with|font-family|tt|language|verbatim|reliability_polynomial>|<pageref|auto-242>>

    <index-1|<with|font-family|tt|reverse_graph>|<pageref|auto-84>>

    <index-1|<with|font-family|tt|language|verbatim|seidel_spectrum>|<pageref|auto-224>>

    <index-1|<with|font-family|tt|seidel_switch>|<pageref|auto-82>>

    <index-1|<with|font-family|tt|language|verbatim|sequence_graph>|<pageref|auto-30>>

    <index-1|<with|font-family|tt|set_edge_attribute>|<pageref|auto-160>>

    <index-1|<with|font-family|tt|set_edge_weight>|<pageref|auto-143>>

    <index-1|<with|font-family|tt|set_graph_attribute>|<pageref|auto-150>>

    <index-1|<with|font-family|tt|set_vertex_attribute>|<pageref|auto-155>>

    <index-1|<with|font-family|tt|language|verbatim|set_vertex_positions>|<pageref|auto-378>>

    <index-1|<with|font-family|tt|shortest_path>|<pageref|auto-351>>

    <index-1|<with|font-family|tt|language|verbatim|sierpinski_graph>|<pageref|auto-56>>

    <index-1|<with|font-family|tt|spanning_tree>|<pageref|auto-359>>

    <index-1|<with|font-family|tt|language|verbatim|st_ordering>|<pageref|auto-301>>

    <index-1|<with|font-family|tt|language|verbatim|star_graph>|<pageref|auto-43>>

    <index-1|<with|font-family|tt|language|verbatim|strongly_connected_components>|<pageref|auto-258>>

    <index-1|<with|font-family|tt|subdivide_edges>|<pageref|auto-147>>

    <index-1|<with|font-family|tt|subgraph>|<pageref|auto-70>>

    <index-1|<with|font-family|tt|tensor_product>|<pageref|auto-95>>

    <index-1|<with|font-family|tt|language|verbatim|topologic_sort>|<pageref|auto-298>>

    <index-1|<with|font-family|tt|language|verbatim|topological_sort>|<pageref|auto-299>>

    <index-1|<with|font-family|tt|language|verbatim|torus_grid_graph>|<pageref|auto-54>>

    <index-1|<with|font-family|tt|language|verbatim|trail>|<pageref|auto-20>>

    <index-1|<with|font-family|tt|language|verbatim|trail2edges>|<pageref|auto-21>>

    <index-1|<with|font-family|tt|language|verbatim|transitive_closure>|<pageref|auto-97>>

    <index-1|<with|font-family|tt|language|verbatim|traveling_salesman>|<pageref|auto-356>>

    <index-1|<with|font-family|tt|language|verbatim|tree_height>|<pageref|auto-273>>

    <index-1|<with|font-family|tt|language|verbatim|truncate_graph>|<pageref|auto-104>>

    <index-1|<with|font-family|tt|language|verbatim|tutte_polynomial>|<pageref|auto-236>>

    <index-1|<with|font-family|tt|language|verbatim|two_edge_connected_components>|<pageref|auto-266>>

    <index-1|<with|font-family|tt|underlying_graph>|<pageref|auto-74>>

    <index-1|<with|font-family|tt|language|verbatim|vertex_connectivity>|<pageref|auto-252>>

    <index-1|<with|font-family|tt|language|verbatim|vertex_degree>|<pageref|auto-188>>

    <index-1|<with|font-family|tt|language|verbatim|vertex_distance>|<pageref|auto-287>>

    <index-1|<with|font-family|tt|language|verbatim|vertex_in_degree>|<pageref|auto-190>>

    <index-1|<with|font-family|tt|language|verbatim|vertex_out_degree>|<pageref|auto-189>>

    <index-1|<with|font-family|tt|language|verbatim|vertices>|<pageref|auto-180>>

    <index-1|<with|font-family|tt|language|verbatim|web_graph>|<pageref|auto-47>>

    <index-1|<with|font-family|tt|language|verbatim|weight_matrix>|<pageref|auto-218>>

    <index-1|<with|font-family|tt|language|verbatim|wheel_graph>|<pageref|auto-45>>
  </the-index>
</body>

<\initial>
  <\collection>
    <associate|font|roman>
    <associate|font-base-size|10>
    <associate|math-font|roman>
    <associate|page-breaking|professional>
    <associate|page-medium|papyrus>
    <associate|page-type|a4>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|a3|<tuple|A3|128>>
    <associate|allpairs-distance|<tuple|4.8.2|96>>
    <associate|articulation-points|<tuple|4.5.5|86>>
    <associate|assign-edge-weights|<tuple|1.10.9|47>>
    <associate|auto-1|<tuple|?|7>>
    <associate|auto-10|<tuple|2|11>>
    <associate|auto-100|<tuple|1.9.11|35>>
    <associate|auto-101|<tuple|1.9.11|35>>
    <associate|auto-102|<tuple|1.9.11|35>>
    <associate|auto-103|<tuple|1.9.12|36>>
    <associate|auto-104|<tuple|1.9.12|36>>
    <associate|auto-105|<tuple|1.10|37>>
    <associate|auto-106|<tuple|1.10.1|37>>
    <associate|auto-107|<tuple|1.10.1|37>>
    <associate|auto-108|<tuple|1.10.1|37>>
    <associate|auto-109|<tuple|1.10.2|40>>
    <associate|auto-11|<tuple|3|11>>
    <associate|auto-110|<tuple|1.10.2|40>>
    <associate|auto-111|<tuple|1.10.3|41>>
    <associate|auto-112|<tuple|1.10.3|41>>
    <associate|auto-113|<tuple|1.10.4|43>>
    <associate|auto-114|<tuple|1.10.4|43>>
    <associate|auto-115|<tuple|1.10.5|44>>
    <associate|auto-116|<tuple|1.10.5|44>>
    <associate|auto-117|<tuple|1.10.6|45>>
    <associate|auto-118|<tuple|1.10.6|45>>
    <associate|auto-119|<tuple|1.10.7|46>>
    <associate|auto-12|<tuple|4|12>>
    <associate|auto-120|<tuple|1.10.7|46>>
    <associate|auto-121|<tuple|1.10.8|46>>
    <associate|auto-122|<tuple|1.10.8|46>>
    <associate|auto-123|<tuple|1.10.9|47>>
    <associate|auto-124|<tuple|1.10.9|47>>
    <associate|auto-125|<tuple|2|49>>
    <associate|auto-126|<tuple|2.1|49>>
    <associate|auto-127|<tuple|2.1.1|49>>
    <associate|auto-128|<tuple|2.1.1|49>>
    <associate|auto-129|<tuple|2.1.2|49>>
    <associate|auto-13|<tuple|5|12>>
    <associate|auto-130|<tuple|2.1.2|49>>
    <associate|auto-131|<tuple|2.2|49>>
    <associate|auto-132|<tuple|2.2.1|49>>
    <associate|auto-133|<tuple|2.2.1|49>>
    <associate|auto-134|<tuple|2.2.1|49>>
    <associate|auto-135|<tuple|2.3|50>>
    <associate|auto-136|<tuple|2.3.1|50>>
    <associate|auto-137|<tuple|2.3.1|50>>
    <associate|auto-138|<tuple|2.3.1|50>>
    <associate|auto-139|<tuple|2.3.1|50>>
    <associate|auto-14|<tuple|1.2|13>>
    <associate|auto-140|<tuple|2.3.1|50>>
    <associate|auto-141|<tuple|2.3.2|51>>
    <associate|auto-142|<tuple|2.3.2|51>>
    <associate|auto-143|<tuple|2.3.2|51>>
    <associate|auto-144|<tuple|2.3.3|52>>
    <associate|auto-145|<tuple|2.3.3|52>>
    <associate|auto-146|<tuple|2.3.4|52>>
    <associate|auto-147|<tuple|2.3.4|52>>
    <associate|auto-148|<tuple|2.4|53>>
    <associate|auto-149|<tuple|2.4.1|53>>
    <associate|auto-15|<tuple|1.2.1|13>>
    <associate|auto-150|<tuple|2.4.1|53>>
    <associate|auto-151|<tuple|2.4.1|53>>
    <associate|auto-152|<tuple|2.4.1|53>>
    <associate|auto-153|<tuple|2.4.1|53>>
    <associate|auto-154|<tuple|2.4.2|54>>
    <associate|auto-155|<tuple|2.4.2|54>>
    <associate|auto-156|<tuple|2.4.2|54>>
    <associate|auto-157|<tuple|2.4.2|54>>
    <associate|auto-158|<tuple|2.4.2|54>>
    <associate|auto-159|<tuple|2.4.3|55>>
    <associate|auto-16|<tuple|1.2.1|13>>
    <associate|auto-160|<tuple|2.4.3|55>>
    <associate|auto-161|<tuple|2.4.3|55>>
    <associate|auto-162|<tuple|2.4.3|55>>
    <associate|auto-163|<tuple|2.4.3|55>>
    <associate|auto-164|<tuple|3|57>>
    <associate|auto-165|<tuple|3.1|57>>
    <associate|auto-166|<tuple|3.1.1|57>>
    <associate|auto-167|<tuple|3.1.1|57>>
    <associate|auto-168|<tuple|3.1.2|57>>
    <associate|auto-169|<tuple|3.2|58>>
    <associate|auto-17|<tuple|1.2.2|13>>
    <associate|auto-170|<tuple|3.2.1|58>>
    <associate|auto-171|<tuple|3.2.1|58>>
    <associate|auto-172|<tuple|3.2.2|59>>
    <associate|auto-173|<tuple|3.1|59>>
    <associate|auto-174|<tuple|4|61>>
    <associate|auto-175|<tuple|4.1|61>>
    <associate|auto-176|<tuple|4.1.1|61>>
    <associate|auto-177|<tuple|4.1.1|61>>
    <associate|auto-178|<tuple|4.1.1|61>>
    <associate|auto-179|<tuple|4.1.2|61>>
    <associate|auto-18|<tuple|1.2.2|13>>
    <associate|auto-180|<tuple|4.1.2|61>>
    <associate|auto-181|<tuple|4.1.2|61>>
    <associate|auto-182|<tuple|4.1.2|61>>
    <associate|auto-183|<tuple|4.1.2|61>>
    <associate|auto-184|<tuple|4.1.2|61>>
    <associate|auto-185|<tuple|4.1.3|62>>
    <associate|auto-186|<tuple|4.1.3|62>>
    <associate|auto-187|<tuple|4.1.4|63>>
    <associate|auto-188|<tuple|4.1.4|63>>
    <associate|auto-189|<tuple|4.1.4|63>>
    <associate|auto-19|<tuple|1.2.3|14>>
    <associate|auto-190|<tuple|4.1.4|63>>
    <associate|auto-191|<tuple|4.1.4|63>>
    <associate|auto-192|<tuple|4.1.4|63>>
    <associate|auto-193|<tuple|4.1.4|63>>
    <associate|auto-194|<tuple|4.1.5|64>>
    <associate|auto-195|<tuple|4.1.5|64>>
    <associate|auto-196|<tuple|4.1.6|65>>
    <associate|auto-197|<tuple|4.1.6|65>>
    <associate|auto-198|<tuple|4.1.7|66>>
    <associate|auto-199|<tuple|4.1.7|66>>
    <associate|auto-2|<tuple|1|9>>
    <associate|auto-20|<tuple|1.2.3|14>>
    <associate|auto-200|<tuple|4.1.7|66>>
    <associate|auto-201|<tuple|4.1.7|66>>
    <associate|auto-202|<tuple|4.1.7|66>>
    <associate|auto-203|<tuple|4.1.7|66>>
    <associate|auto-204|<tuple|4.1.8|67>>
    <associate|auto-205|<tuple|4.1.8|67>>
    <associate|auto-206|<tuple|4.1.9|68>>
    <associate|auto-207|<tuple|4.1.9|68>>
    <associate|auto-208|<tuple|4.1.10|68>>
    <associate|auto-209|<tuple|4.1.10|68>>
    <associate|auto-21|<tuple|1.2.3|14>>
    <associate|auto-210|<tuple|4.2|69>>
    <associate|auto-211|<tuple|4.2.1|69>>
    <associate|auto-212|<tuple|4.2.1|69>>
    <associate|auto-213|<tuple|4.2.2|70>>
    <associate|auto-214|<tuple|4.2.2|70>>
    <associate|auto-215|<tuple|4.2.3|71>>
    <associate|auto-216|<tuple|4.2.3|71>>
    <associate|auto-217|<tuple|4.2.4|72>>
    <associate|auto-218|<tuple|4.2.4|72>>
    <associate|auto-219|<tuple|4.2.5|73>>
    <associate|auto-22|<tuple|1.3|14>>
    <associate|auto-220|<tuple|4.2.5|73>>
    <associate|auto-221|<tuple|4.2.6|73>>
    <associate|auto-222|<tuple|4.2.6|73>>
    <associate|auto-223|<tuple|4.2.7|74>>
    <associate|auto-224|<tuple|4.2.7|74>>
    <associate|auto-225|<tuple|4.2.8|74>>
    <associate|auto-226|<tuple|4.2.8|74>>
    <associate|auto-227|<tuple|4.3|75>>
    <associate|auto-228|<tuple|4.3.1|75>>
    <associate|auto-229|<tuple|4.3.1|75>>
    <associate|auto-23|<tuple|1.3.1|14>>
    <associate|auto-230|<tuple|4.3.2|77>>
    <associate|auto-231|<tuple|4.3.2|77>>
    <associate|auto-232|<tuple|4.3.3|78>>
    <associate|auto-233|<tuple|4.3.3|78>>
    <associate|auto-234|<tuple|4.4|78>>
    <associate|auto-235|<tuple|4.4.1|78>>
    <associate|auto-236|<tuple|4.4.1|78>>
    <associate|auto-237|<tuple|4.4.2|80>>
    <associate|auto-238|<tuple|4.4.2|80>>
    <associate|auto-239|<tuple|4.4.3|81>>
    <associate|auto-24|<tuple|1.3.1|14>>
    <associate|auto-240|<tuple|4.4.3|81>>
    <associate|auto-241|<tuple|4.4.4|81>>
    <associate|auto-242|<tuple|4.4.4|81>>
    <associate|auto-243|<tuple|4.5|83>>
    <associate|auto-244|<tuple|4.5.1|83>>
    <associate|auto-245|<tuple|4.5.1|83>>
    <associate|auto-246|<tuple|4.5.1|83>>
    <associate|auto-247|<tuple|4.5.1|83>>
    <associate|auto-248|<tuple|4.5.2|84>>
    <associate|auto-249|<tuple|4.5.2|84>>
    <associate|auto-25|<tuple|1.3.2|15>>
    <associate|auto-250|<tuple|4.5.2|84>>
    <associate|auto-251|<tuple|4.5.3|85>>
    <associate|auto-252|<tuple|4.5.3|85>>
    <associate|auto-253|<tuple|4.5.4|86>>
    <associate|auto-254|<tuple|4.5.4|86>>
    <associate|auto-255|<tuple|4.5.5|86>>
    <associate|auto-256|<tuple|4.5.5|86>>
    <associate|auto-257|<tuple|4.5.6|86>>
    <associate|auto-258|<tuple|4.5.6|86>>
    <associate|auto-259|<tuple|4.5.6|86>>
    <associate|auto-26|<tuple|1.3.2|15>>
    <associate|auto-260|<tuple|4.5.7|87>>
    <associate|auto-261|<tuple|4.5.7|87>>
    <associate|auto-262|<tuple|4.5.8|88>>
    <associate|auto-263|<tuple|4.5.8|88>>
    <associate|auto-264|<tuple|4.5.9|89>>
    <associate|auto-265|<tuple|4.5.9|89>>
    <associate|auto-266|<tuple|4.5.9|89>>
    <associate|auto-267|<tuple|4.6|90>>
    <associate|auto-268|<tuple|4.6.1|90>>
    <associate|auto-269|<tuple|4.6.1|90>>
    <associate|auto-27|<tuple|1.3.2|15>>
    <associate|auto-270|<tuple|4.6.2|90>>
    <associate|auto-271|<tuple|4.6.2|90>>
    <associate|auto-272|<tuple|4.6.3|90>>
    <associate|auto-273|<tuple|4.6.3|90>>
    <associate|auto-274|<tuple|4.6.4|91>>
    <associate|auto-275|<tuple|4.6.4|91>>
    <associate|auto-276|<tuple|4.6.5|91>>
    <associate|auto-277|<tuple|4.6.5|91>>
    <associate|auto-278|<tuple|4.7|92>>
    <associate|auto-279|<tuple|4.7.1|92>>
    <associate|auto-28|<tuple|1.4|16>>
    <associate|auto-280|<tuple|4.7.1|92>>
    <associate|auto-281|<tuple|4.7.2|93>>
    <associate|auto-282|<tuple|4.7.2|93>>
    <associate|auto-283|<tuple|4.7.3|94>>
    <associate|auto-284|<tuple|4.7.3|94>>
    <associate|auto-285|<tuple|4.8|95>>
    <associate|auto-286|<tuple|4.8.1|95>>
    <associate|auto-287|<tuple|4.8.1|95>>
    <associate|auto-288|<tuple|4.8.2|96>>
    <associate|auto-289|<tuple|4.8.2|96>>
    <associate|auto-29|<tuple|1.4.1|16>>
    <associate|auto-290|<tuple|4.8.3|97>>
    <associate|auto-291|<tuple|4.8.4|97>>
    <associate|auto-292|<tuple|4.8.4|97>>
    <associate|auto-293|<tuple|4.8.4|97>>
    <associate|auto-294|<tuple|4.9|98>>
    <associate|auto-295|<tuple|4.9.1|98>>
    <associate|auto-296|<tuple|4.9.1|98>>
    <associate|auto-297|<tuple|4.9.2|98>>
    <associate|auto-298|<tuple|4.9.2|98>>
    <associate|auto-299|<tuple|4.9.2|98>>
    <associate|auto-3|<tuple|1.1|9>>
    <associate|auto-30|<tuple|1.4.1|16>>
    <associate|auto-300|<tuple|4.9.3|99>>
    <associate|auto-301|<tuple|4.9.3|99>>
    <associate|auto-302|<tuple|4.10|100>>
    <associate|auto-303|<tuple|4.10.1|100>>
    <associate|auto-304|<tuple|4.10.1|100>>
    <associate|auto-305|<tuple|4.10.2|101>>
    <associate|auto-306|<tuple|4.10.2|101>>
    <associate|auto-307|<tuple|4.11|102>>
    <associate|auto-308|<tuple|4.11.1|102>>
    <associate|auto-309|<tuple|4.11.1|102>>
    <associate|auto-31|<tuple|1.4.2|16>>
    <associate|auto-310|<tuple|4.11.2|102>>
    <associate|auto-311|<tuple|4.11.2|102>>
    <associate|auto-312|<tuple|4.11.3|104>>
    <associate|auto-313|<tuple|4.11.3|104>>
    <associate|auto-314|<tuple|4.11.3|104>>
    <associate|auto-315|<tuple|4.11.4|104>>
    <associate|auto-316|<tuple|4.11.4|104>>
    <associate|auto-317|<tuple|4.11.5|105>>
    <associate|auto-318|<tuple|4.11.5|105>>
    <associate|auto-319|<tuple|4.12|106>>
    <associate|auto-32|<tuple|1.4.2|16>>
    <associate|auto-320|<tuple|4.12.1|106>>
    <associate|auto-321|<tuple|4.12.1|106>>
    <associate|auto-322|<tuple|4.12.2|107>>
    <associate|auto-323|<tuple|4.12.2|107>>
    <associate|auto-324|<tuple|4.12.3|109>>
    <associate|auto-325|<tuple|4.12.3|109>>
    <associate|auto-326|<tuple|4.13|110>>
    <associate|auto-327|<tuple|4.13.1|110>>
    <associate|auto-328|<tuple|4.13.1|110>>
    <associate|auto-329|<tuple|4.1|111>>
    <associate|auto-33|<tuple|1.5|16>>
    <associate|auto-330|<tuple|4.13.2|111>>
    <associate|auto-331|<tuple|4.13.2|111>>
    <associate|auto-332|<tuple|4.13.3|111>>
    <associate|auto-333|<tuple|4.13.3|111>>
    <associate|auto-334|<tuple|4.13.4|112>>
    <associate|auto-335|<tuple|4.13.4|112>>
    <associate|auto-336|<tuple|4.13.5|113>>
    <associate|auto-337|<tuple|4.13.5|113>>
    <associate|auto-338|<tuple|4.14|114>>
    <associate|auto-339|<tuple|4.14.1|114>>
    <associate|auto-34|<tuple|1.5.1|16>>
    <associate|auto-340|<tuple|4.14.1|114>>
    <associate|auto-341|<tuple|4.14.2|115>>
    <associate|auto-342|<tuple|4.14.2|115>>
    <associate|auto-343|<tuple|5|117>>
    <associate|auto-344|<tuple|5.1|117>>
    <associate|auto-345|<tuple|5.1.1|117>>
    <associate|auto-346|<tuple|5.1.1|117>>
    <associate|auto-347|<tuple|5.1.2|117>>
    <associate|auto-348|<tuple|5.1.2|117>>
    <associate|auto-349|<tuple|5.2|118>>
    <associate|auto-35|<tuple|1.5.1|16>>
    <associate|auto-350|<tuple|5.2.1|118>>
    <associate|auto-351|<tuple|5.2.1|118>>
    <associate|auto-352|<tuple|5.2.2|119>>
    <associate|auto-353|<tuple|5.2.2|119>>
    <associate|auto-354|<tuple|5.2.2|119>>
    <associate|auto-355|<tuple|5.2.3|120>>
    <associate|auto-356|<tuple|5.2.3|120>>
    <associate|auto-357|<tuple|5.3|122>>
    <associate|auto-358|<tuple|5.3.1|122>>
    <associate|auto-359|<tuple|5.3.1|122>>
    <associate|auto-36|<tuple|1.5.2|17>>
    <associate|auto-360|<tuple|5.3.2|123>>
    <associate|auto-361|<tuple|5.3.2|123>>
    <associate|auto-362|<tuple|5.3.3|124>>
    <associate|auto-363|<tuple|5.3.3|124>>
    <associate|auto-364|<tuple|6|125>>
    <associate|auto-365|<tuple|6.1|125>>
    <associate|auto-366|<tuple|6.1|125>>
    <associate|auto-367|<tuple|6.1.1|125>>
    <associate|auto-368|<tuple|6.1.2|126>>
    <associate|auto-369|<tuple|6.1.3|126>>
    <associate|auto-37|<tuple|1.5.2|17>>
    <associate|auto-370|<tuple|6.1.4|128>>
    <associate|auto-371|<tuple|6.1.5|129>>
    <associate|auto-372|<tuple|6.1|129>>
    <associate|auto-373|<tuple|6.2|130>>
    <associate|auto-374|<tuple|6.3|130>>
    <associate|auto-375|<tuple|6.1.6|131>>
    <associate|auto-376|<tuple|6.2|131>>
    <associate|auto-377|<tuple|6.2.1|131>>
    <associate|auto-378|<tuple|6.2.1|131>>
    <associate|auto-379|<tuple|6.2.2|132>>
    <associate|auto-38|<tuple|1.5.2|17>>
    <associate|auto-380|<tuple|6.3|133>>
    <associate|auto-381|<tuple|6.3.1|133>>
    <associate|auto-382|<tuple|6.3.1|133>>
    <associate|auto-383|<tuple|6.3.2|133>>
    <associate|auto-384|<tuple|6.3.2|133>>
    <associate|auto-385|<tuple|6.3.2|133>>
    <associate|auto-386|<tuple|6.3.3|134>>
    <associate|auto-387|<tuple|6.3.3|134>>
    <associate|auto-388|<tuple|6.3.3|137>>
    <associate|auto-389|<tuple|65|139>>
    <associate|auto-39|<tuple|1.6|17>>
    <associate|auto-4|<tuple|1.1.1|9>>
    <associate|auto-40|<tuple|1.6.1|17>>
    <associate|auto-41|<tuple|1.6.1|17>>
    <associate|auto-42|<tuple|1.6.2|18>>
    <associate|auto-43|<tuple|1.6.2|18>>
    <associate|auto-44|<tuple|1.6.3|18>>
    <associate|auto-45|<tuple|1.6.3|18>>
    <associate|auto-46|<tuple|1.6.4|19>>
    <associate|auto-47|<tuple|1.6.4|19>>
    <associate|auto-48|<tuple|1.6.5|19>>
    <associate|auto-49|<tuple|1.6.5|19>>
    <associate|auto-5|<tuple|1.1.1|9>>
    <associate|auto-50|<tuple|1.6.6|19>>
    <associate|auto-51|<tuple|1.6.6|19>>
    <associate|auto-52|<tuple|1.6.7|20>>
    <associate|auto-53|<tuple|1.6.7|20>>
    <associate|auto-54|<tuple|1.6.7|20>>
    <associate|auto-55|<tuple|1.6.8|21>>
    <associate|auto-56|<tuple|1.6.8|21>>
    <associate|auto-57|<tuple|1.6.9|22>>
    <associate|auto-58|<tuple|1.6.9|22>>
    <associate|auto-59|<tuple|1.6.10|23>>
    <associate|auto-6|<tuple|1.1.2|10>>
    <associate|auto-60|<tuple|1.6.10|23>>
    <associate|auto-61|<tuple|1.7|23>>
    <associate|auto-62|<tuple|1.7.1|23>>
    <associate|auto-63|<tuple|1.7.1|23>>
    <associate|auto-64|<tuple|1.7.2|24>>
    <associate|auto-65|<tuple|1.7.2|24>>
    <associate|auto-66|<tuple|1.7.3|25>>
    <associate|auto-67|<tuple|1.7.3|25>>
    <associate|auto-68|<tuple|1.8|25>>
    <associate|auto-69|<tuple|1.8.1|25>>
    <associate|auto-7|<tuple|1.1.2|10>>
    <associate|auto-70|<tuple|1.8.1|25>>
    <associate|auto-71|<tuple|1.8.2|26>>
    <associate|auto-72|<tuple|1.8.2|26>>
    <associate|auto-73|<tuple|1.8.3|26>>
    <associate|auto-74|<tuple|1.8.3|26>>
    <associate|auto-75|<tuple|1.8.4|27>>
    <associate|auto-76|<tuple|1.8.4|27>>
    <associate|auto-77|<tuple|1.8.4|27>>
    <associate|auto-78|<tuple|1.9|29>>
    <associate|auto-79|<tuple|1.9.1|29>>
    <associate|auto-8|<tuple|1.1.3|10>>
    <associate|auto-80|<tuple|1.9.1|29>>
    <associate|auto-81|<tuple|1.9.2|29>>
    <associate|auto-82|<tuple|1.9.2|29>>
    <associate|auto-83|<tuple|1.9.3|30>>
    <associate|auto-84|<tuple|1.9.3|30>>
    <associate|auto-85|<tuple|1.9.4|30>>
    <associate|auto-86|<tuple|1.9.4|30>>
    <associate|auto-87|<tuple|1.9.5|31>>
    <associate|auto-88|<tuple|1.9.5|31>>
    <associate|auto-89|<tuple|1.9.6|31>>
    <associate|auto-9|<tuple|1|10>>
    <associate|auto-90|<tuple|1.9.6|31>>
    <associate|auto-91|<tuple|1.9.7|31>>
    <associate|auto-92|<tuple|1.9.7|31>>
    <associate|auto-93|<tuple|1.9.8|32>>
    <associate|auto-94|<tuple|1.9.8|32>>
    <associate|auto-95|<tuple|1.9.8|32>>
    <associate|auto-96|<tuple|1.9.9|33>>
    <associate|auto-97|<tuple|1.9.9|33>>
    <associate|auto-98|<tuple|1.9.10|35>>
    <associate|auto-99|<tuple|1.9.10|35>>
    <associate|automorphisms|<tuple|4.3.3|78>>
    <associate|bib-afzal|<tuple|1|137>>
    <associate|bib-alonso|<tuple|2|137>>
    <associate|bib-andova|<tuple|3|137>>
    <associate|bib-bagatelj|<tuple|4|137>>
    <associate|bib-bayati|<tuple|5|137>>
    <associate|bib-bellman|<tuple|6|137>>
    <associate|bib-biggs|<tuple|7|137>>
    <associate|bib-blanusa|<tuple|8|137>>
    <associate|bib-bollobas|<tuple|9|137>>
    <associate|bib-boot|<tuple|10|137>>
    <associate|bib-brelaz|<tuple|11|137>>
    <associate|bib-buchheim|<tuple|12|137>>
    <associate|bib-christofides|<tuple|13|137>>
    <associate|bib-cook|<tuple|14|137>>
    <associate|bib-cpap|<tuple|48|138>>
    <associate|bib-diaz|<tuple|16|137>>
    <associate|bib-diestel|<tuple|17|137>>
    <associate|bib-dijkstra|<tuple|18|137>>
    <associate|bib-edmonds|<tuple|19|137>>
    <associate|bib-edmonds-karp|<tuple|20|137>>
    <associate|bib-esfahanian|<tuple|21|137>>
    <associate|bib-even-algorithms|<tuple|22|137>>
    <associate|bib-floyd|<tuple|23|137>>
    <associate|bib-ford|<tuple|24|137>>
    <associate|bib-fruchterman|<tuple|25|137>>
    <associate|bib-gibbons|<tuple|26|137>>
    <associate|bib-godsil|<tuple|27|137>>
    <associate|bib-goldfarb|<tuple|28|137>>
    <associate|bib-haggard|<tuple|29|137>>
    <associate|bib-haggard2|<tuple|30|137>>
    <associate|bib-hakimi|<tuple|31|137>>
    <associate|bib-helsgaun|<tuple|32|137>>
    <associate|bib-hierholzer|<tuple|33|137>>
    <associate|bib-hinz|<tuple|34|137>>
    <associate|bib-hopcroft|<tuple|35|138>>
    <associate|bib-hu|<tuple|36|138>>
    <associate|bib-hu2|<tuple|37|138>>
    <associate|bib-kahn|<tuple|38|138>>
    <associate|bib-krishnaiyan|<tuple|58|138>>
    <associate|bib-latapy|<tuple|39|138>>
    <associate|bib-lca|<tuple|56|138>>
    <associate|bib-mckay|<tuple|40|138>>
    <associate|bib-melissa|<tuple|15|137>>
    <associate|bib-monagan|<tuple|41|138>>
    <associate|bib-myrwold|<tuple|42|138>>
    <associate|bib-newman|<tuple|43|138>>
    <associate|bib-nijenhuis|<tuple|44|138>>
    <associate|bib-ostergard|<tuple|45|138>>
    <associate|bib-otter|<tuple|46|138>>
    <associate|bib-padberg|<tuple|47|138>>
    <associate|bib-pferschy|<tuple|49|138>>
    <associate|bib-plestenjak|<tuple|50|138>>
    <associate|bib-schank|<tuple|52|138>>
    <associate|bib-schank2|<tuple|51|138>>
    <associate|bib-steger|<tuple|53|138>>
    <associate|bib-tarjan-bridges|<tuple|55|138>>
    <associate|bib-tarjan72|<tuple|54|138>>
    <associate|bib-tarjan86|<tuple|57|138>>
    <associate|bib-tomita|<tuple|59|138>>
    <associate|bib-tutte|<tuple|60|138>>
    <associate|bib-walker|<tuple|61|138>>
    <associate|bib-wasserman|<tuple|62|138>>
    <associate|bib-welch|<tuple|63|138>>
    <associate|bib-west|<tuple|64|138>>
    <associate|bib-wilf|<tuple|65|138>>
    <associate|blockjoin|<tuple|6.1|129>>
    <associate|canonical-labeling|<tuple|4.3.2|77>>
    <associate|cheapest-paths|<tuple|5.2.2|119>>
    <associate|chordface|<tuple|6.2|130>>
    <associate|circle-method|<tuple|6.1.6|131>>
    <associate|clustering-coefficient|<tuple|4.12.2|107>>
    <associate|connected-components|<tuple|4.5.2|84>>
    <associate|departures-arrivals|<tuple|4.1.7|66>>
    <associate|digraph|<tuple|1.1.2|10>>
    <associate|draw-graph|<tuple|6.1|125>>
    <associate|eq:chromatic-poly|<tuple|4.1|80>>
    <associate|eq:flow-poly|<tuple|4.2|81>>
    <associate|eq:reliability-poly|<tuple|4.3|82>>
    <associate|footnote-1|<tuple|1|7>>
    <associate|footnote-1.1|<tuple|1.1|41>>
    <associate|footnote-2|<tuple|2|7>>
    <associate|footnote-3.1|<tuple|3.1|57>>
    <associate|footnote-3.2|<tuple|3.2|59>>
    <associate|footnote-4.1|<tuple|4.1|79>>
    <associate|footnote-5.1|<tuple|5.1|120>>
    <associate|footnr-1|<tuple|1|7>>
    <associate|footnr-1.1|<tuple|1.1|41>>
    <associate|footnr-2|<tuple|2|7>>
    <associate|footnr-3.1|<tuple|3.1|57>>
    <associate|footnr-3.2|<tuple|3.2|59>>
    <associate|footnr-4.1|<tuple|4.1|79>>
    <associate|footnr-5.1|<tuple|5.1|120>>
    <associate|frucht|<tuple|1.6.10|23>>
    <associate|graph|<tuple|1.1.1|9>>
    <associate|graph-equal|<tuple|4.1.3|62>>
    <associate|graph-products|<tuple|1.9.8|32>>
    <associate|graph-spectrum|<tuple|4.2.6|73>>
    <associate|graph-union|<tuple|1.9.4|30>>
    <associate|highlight-subgraph|<tuple|6.3.3|134>>
    <associate|highlight-trail|<tuple|6.3.2|133>>
    <associate|highlight-vertices|<tuple|6.3.1|133>>
    <associate|import-graph|<tuple|3.1.1|57>>
    <associate|induced-subgraph|<tuple|1.8.2|26>>
    <associate|is-bipartite|<tuple|4.1.9|68>>
    <associate|is-isomorphic|<tuple|4.3.1|75>>
    <associate|is-network|<tuple|4.7.1|92>>
    <associate|is-regular|<tuple|4.1.5|64>>
    <associate|is-tournament|<tuple|4.1.8|67>>
    <associate|is-tree|<tuple|4.6.1|90>>
    <associate|isomorphic-copy|<tuple|1.7.1|23>>
    <associate|laplacian-matrix|<tuple|4.2.2|70>>
    <associate|make-directed|<tuple|2.1.1|49>>
    <associate|make-weighted|<tuple|2.1.2|49>>
    <associate|maxflow|<tuple|4.7.2|93>>
    <associate|maximum-matching|<tuple|4.10.1|100>>
    <associate|minimal-coloring|<tuple|4.13.2|111>>
    <associate|minimum-covering|<tuple|4.11.4|104>>
    <associate|number-of-spanning-trees|<tuple|5.3.3|124>>
    <associate|random-graph|<tuple|1.10.1|37>>
    <associate|random-network|<tuple|1.10.8|46>>
    <associate|random-planar|<tuple|1.10.4|43>>
    <associate|set-vertex-positions|<tuple|6.2.1|131>>
    <associate|st-ordering|<tuple|4.9.3|99>>
    <associate|st53|<tuple|3.1|59>>
    <associate|subgraph|<tuple|1.8.1|25>>
    <associate|tab:colors|<tuple|4.1|111>>
    <associate|touchface|<tuple|6.3|130>>
    <associate|trail|<tuple|1.2.3|14>>
    <associate|traveling-salesman|<tuple|5.2.3|120>>
    <associate|tutte-polynomial|<tuple|4.4.1|78>>
    <associate|underlying-graph|<tuple|1.8.3|26>>
    <associate|vertex-attribute|<tuple|2.4.2|54>>
    <associate|vertex-distance|<tuple|4.8.1|95>>
    <associate|vertices-edges|<tuple|4.1.2|61>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|bib>
      godsil

      godsil

      godsil

      hakimi

      afzal

      hinz

      tomita

      godsil

      godsil

      godsil

      gibbons

      andova

      andova

      bagatelj

      newman

      hakimi

      schank

      nijenhuis

      alonso

      wilf

      otter

      wilf

      nijenhuis

      bayati

      steger

      goldfarb

      godsil

      mckay

      godsil

      biggs

      haggard

      biggs

      bollobas

      haggard

      haggard2

      monagan

      biggs

      bollobas

      haggard

      bollobas

      haggard

      biggs

      bollobas

      monagan

      godsil

      gibbons

      tarjan72

      tarjan72

      esfahanian

      even-algorithms

      gibbons

      tarjan72

      krishnaiyan

      tarjan-bridges

      lca

      edmonds-karp

      gibbons

      floyd

      kahn

      tarjan86

      cpap

      godsil

      edmonds

      hopcroft

      tomita

      ostergard

      schank2

      latapy

      boot

      boot

      schank

      boot

      wasserman

      boot

      diaz

      brelaz

      brelaz

      brelaz

      diestel

      blanusa

      godsil

      hierholzer

      melissa

      dijkstra

      bellman

      ford

      cook

      pferschy

      padberg

      christofides

      helsgaun

      west

      hu

      fruchterman

      hu2

      welch

      walker

      buchheim

      tutte

      godsil

      gibbons

      gibbons

      myrwold

      plestenjak
    </associate>
    <\associate|figure>
      <tuple|normal|<surround|<hidden|<tuple>>||drawing of the Sierpi«ski
      graph <with|mode|<quote|math>|S T<rsub|3><rsup|5>> using
      L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|<quote|0>|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
      and PSTricks>|<pageref|auto-173>>

      <tuple|normal|<surround|<hidden|<tuple>>||Joining two block by adding a
      temporary edge.>|<pageref|auto-372>>

      <tuple|normal|<surround|<hidden|<tuple>>||A chorded face
      <with|mode|<quote|math>|f>.>|<pageref|auto-373>>

      <tuple|normal|<surround|<hidden|<tuple>>||Faces
      <with|mode|<quote|math>|f> and <with|mode|<quote|math>|g> having two
      vertices but no edges in common.>|<pageref|auto-374>>
    </associate>
    <\associate|idx>
      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph>>|<pageref|auto-5>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|digraph>>|<pageref|auto-7>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|cycle_graph>>|<pageref|auto-16>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|path_graph>>|<pageref|auto-18>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|trail>>|<pageref|auto-20>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|trail2edges>>|<pageref|auto-21>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|complete_graph>>|<pageref|auto-24>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|complete_binary_tree>>|<pageref|auto-26>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|complete_kary_tree>>|<pageref|auto-27>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|sequence_graph>>|<pageref|auto-30>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_graphic_sequence>>|<pageref|auto-32>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|interval_graph>>|<pageref|auto-35>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|kneser_graph>>|<pageref|auto-37>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|odd_graph>>|<pageref|auto-38>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|hypercube_graph>>|<pageref|auto-41>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|star_graph>>|<pageref|auto-43>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|wheel_graph>>|<pageref|auto-45>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|web_graph>>|<pageref|auto-47>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|prism_graph>>|<pageref|auto-49>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|antiprism_graph>>|<pageref|auto-51>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|grid_graph>>|<pageref|auto-53>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|torus_grid_graph>>|<pageref|auto-54>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|sierpinski_graph>>|<pageref|auto-56>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|petersen_graph>>|<pageref|auto-58>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|lcf_graph>>|<pageref|auto-60>>

      <tuple|<tuple|<with|font-family|<quote|tt>|isomorphic_copy>>|<pageref|auto-63>>

      <tuple|<tuple|<with|font-family|<quote|tt>|permute_vertices>>|<pageref|auto-65>>

      <tuple|<tuple|<with|font-family|<quote|tt>|relabel_vertices>>|<pageref|auto-67>>

      <tuple|<tuple|<with|font-family|<quote|tt>|subgraph>>|<pageref|auto-70>>

      <tuple|<tuple|<with|font-family|<quote|tt>|induced_subgraph>>|<pageref|auto-72>>

      <tuple|<tuple|<with|font-family|<quote|tt>|underlying_graph>>|<pageref|auto-74>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|fundamental_cycle>>|<pageref|auto-76>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|cycle_basis>>|<pageref|auto-77>>

      <tuple|<tuple|<with|font-family|<quote|tt>|graph_complement>>|<pageref|auto-80>>

      <tuple|<tuple|<with|font-family|<quote|tt>|seidel_switch>>|<pageref|auto-82>>

      <tuple|<tuple|<with|font-family|<quote|tt>|reverse_graph>>|<pageref|auto-84>>

      <tuple|<tuple|<with|font-family|<quote|tt>|graph_union>>|<pageref|auto-86>>

      <tuple|<tuple|<with|font-family|<quote|tt>|disjoint_union>>|<pageref|auto-88>>

      <tuple|<tuple|<with|font-family|<quote|tt>|graph_join>>|<pageref|auto-90>>

      <tuple|<tuple|<with|font-family|<quote|tt>|graph_power>>|<pageref|auto-92>>

      <tuple|<tuple|<with|font-family|<quote|tt>|cartesian_product>>|<pageref|auto-94>>

      <tuple|<tuple|<with|font-family|<quote|tt>|tensor_product>>|<pageref|auto-95>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|transitive_closure>>|<pageref|auto-97>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|line_graph>>|<pageref|auto-99>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|plane_dual>>|<pageref|auto-101>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_planar>>|<pageref|auto-102>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|truncate_graph>>|<pageref|auto-104>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_graph>>|<pageref|auto-107>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_digraph>>|<pageref|auto-108>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_bipartite_graph>>|<pageref|auto-110>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_tree>>|<pageref|auto-112>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_planar_graph>>|<pageref|auto-114>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_sequence_graph>>|<pageref|auto-116>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_regular_graph>>|<pageref|auto-118>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_tournament>>|<pageref|auto-120>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|random_network>>|<pageref|auto-122>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|assign_edge_weights>>|<pageref|auto-124>>

      <tuple|<tuple|<with|font-family|<quote|tt>|make_directed>>|<pageref|auto-128>>

      <tuple|<tuple|<with|font-family|<quote|tt>|make_weighted>>|<pageref|auto-130>>

      <tuple|<tuple|<with|font-family|<quote|tt>|add_vertex>>|<pageref|auto-133>>

      <tuple|<tuple|<with|font-family|<quote|tt>|delete_vertex>>|<pageref|auto-134>>

      <tuple|<tuple|<with|font-family|<quote|tt>|add_edge>>|<pageref|auto-137>>

      <tuple|<tuple|<with|font-family|<quote|tt>|add_arc>>|<pageref|auto-138>>

      <tuple|<tuple|<with|font-family|<quote|tt>|delete_edge>>|<pageref|auto-139>>

      <tuple|<tuple|<with|font-family|<quote|tt>|delete_arc>>|<pageref|auto-140>>

      <tuple|<tuple|<with|font-family|<quote|tt>|get_edge_weight>>|<pageref|auto-142>>

      <tuple|<tuple|<with|font-family|<quote|tt>|set_edge_weight>>|<pageref|auto-143>>

      <tuple|<tuple|<with|font-family|<quote|tt>|contract_edge>>|<pageref|auto-145>>

      <tuple|<tuple|<with|font-family|<quote|tt>|subdivide_edges>>|<pageref|auto-147>>

      <tuple|<tuple|<with|font-family|<quote|tt>|set_graph_attribute>>|<pageref|auto-150>>

      <tuple|<tuple|<with|font-family|<quote|tt>|get_graph_attribute>>|<pageref|auto-151>>

      <tuple|<tuple|<with|font-family|<quote|tt>|list_graph_attributes>>|<pageref|auto-152>>

      <tuple|<tuple|<with|font-family|<quote|tt>|discard_graph_attribute>>|<pageref|auto-153>>

      <tuple|<tuple|<with|font-family|<quote|tt>|set_vertex_attribute>>|<pageref|auto-155>>

      <tuple|<tuple|<with|font-family|<quote|tt>|get_vertex_attribute>>|<pageref|auto-156>>

      <tuple|<tuple|<with|font-family|<quote|tt>|list_vertex_attributes>>|<pageref|auto-157>>

      <tuple|<tuple|<with|font-family|<quote|tt>|discard_vertex_attribute>>|<pageref|auto-158>>

      <tuple|<tuple|<with|font-family|<quote|tt>|set_edge_attribute>>|<pageref|auto-160>>

      <tuple|<tuple|<with|font-family|<quote|tt>|get_edge_attribute>>|<pageref|auto-161>>

      <tuple|<tuple|<with|font-family|<quote|tt>|list_edge_attributes>>|<pageref|auto-162>>

      <tuple|<tuple|<with|font-family|<quote|tt>|discard_edge_attribute>>|<pageref|auto-163>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|import_graph>>|<pageref|auto-167>>

      <tuple|<tuple|<with|font-family|<quote|tt>|export_graph>>|<pageref|auto-171>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_directed>>|<pageref|auto-177>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_weighted>>|<pageref|auto-178>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertices>>|<pageref|auto-180>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_vertices>>|<pageref|auto-181>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|edges>>|<pageref|auto-182>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|number_of_vertices>>|<pageref|auto-183>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|number_of_edges>>|<pageref|auto-184>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_equal>>|<pageref|auto-186>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertex_degree>>|<pageref|auto-188>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertex_out_degree>>|<pageref|auto-189>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertex_in_degree>>|<pageref|auto-190>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|degree_sequence>>|<pageref|auto-191>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|minimum_degree>>|<pageref|auto-192>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|maximum_degree>>|<pageref|auto-193>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_regular>>|<pageref|auto-195>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_strongly_regular>>|<pageref|auto-197>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|has_edge>>|<pageref|auto-199>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|has_arc>>|<pageref|auto-200>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|neighbors>>|<pageref|auto-201>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|departures>>|<pageref|auto-202>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|arrivals>>|<pageref|auto-203>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_tournament>>|<pageref|auto-205>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_bipartite>>|<pageref|auto-207>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|incident_edges>>|<pageref|auto-209>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|adjacency_matrix>>|<pageref|auto-212>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|laplacian_matrix>>|<pageref|auto-214>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|incidence_matrix>>|<pageref|auto-216>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|weight_matrix>>|<pageref|auto-218>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_charpoly>>|<pageref|auto-220>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_spectrum>>|<pageref|auto-222>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|seidel_spectrum>>|<pageref|auto-224>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_integer_graph>>|<pageref|auto-226>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_isomorphic>>|<pageref|auto-229>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|canonical_labeling>>|<pageref|auto-231>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_automorphisms>>|<pageref|auto-233>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|tutte_polynomial>>|<pageref|auto-236>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|chromatic_polynomial>>|<pageref|auto-238>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|flow_polynomial>>|<pageref|auto-240>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|reliability_polynomial>>|<pageref|auto-242>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_connected>>|<pageref|auto-245>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_biconnected>>|<pageref|auto-246>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_triconnected>>|<pageref|auto-247>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|connected_components>>|<pageref|auto-249>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|biconnected_components>>|<pageref|auto-250>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertex_connectivity>>|<pageref|auto-252>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|graph_rank>>|<pageref|auto-254>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|articulation_points>>|<pageref|auto-256>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|strongly_connected_components>>|<pageref|auto-258>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_strongly_connected>>|<pageref|auto-259>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|edge_connectivity>>|<pageref|auto-261>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_cut_set>>|<pageref|auto-263>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_two_edge_connected>>|<pageref|auto-265>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|two_edge_connected_components>>|<pageref|auto-266>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_tree>>|<pageref|auto-269>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_forest>>|<pageref|auto-271>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|tree_height>>|<pageref|auto-273>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|lowest_common_ancestor>>|<pageref|auto-275>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_arborescence>>|<pageref|auto-277>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_network>>|<pageref|auto-280>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|maxflow>>|<pageref|auto-282>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|minimum_cut>>|<pageref|auto-284>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|vertex_distance>>|<pageref|auto-287>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|allpairs_distance>>|<pageref|auto-289>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|girth>>|<pageref|auto-292>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|odd_girth>>|<pageref|auto-293>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_acyclic>>|<pageref|auto-296>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|topologic_sort>>|<pageref|auto-298>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|topological_sort>>|<pageref|auto-299>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|st_ordering>>|<pageref|auto-301>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|maximum_matching>>|<pageref|auto-304>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|bipartite_matching>>|<pageref|auto-306>>

      <tuple|<tuple|<with|font-family|<quote|tt>|is_clique>>|<pageref|auto-309>>

      <tuple|<tuple|<with|font-family|<quote|tt>|clique_stats>>|<pageref|auto-311>>

      <tuple|<tuple|<with|font-family|<quote|tt>|maximum_clique>>|<pageref|auto-313>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|clique_number>>|<pageref|auto-314>>

      <tuple|<tuple|<with|font-family|<quote|tt>|clique_cover>>|<pageref|auto-316>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|clique_cover_number>>|<pageref|auto-318>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_triangle_free>>|<pageref|auto-321>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|clustering_coefficient>>|<pageref|auto-323>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|network_transitivity>>|<pageref|auto-325>>

      <tuple|<tuple|<with|font-family|<quote|tt>|greedy_color>>|<pageref|auto-328>>

      <tuple|<tuple|<with|font-family|<quote|tt>|minimal_vertex_coloring>>|<pageref|auto-331>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|chromatic_number>>|<pageref|auto-333>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|mycielski>>|<pageref|auto-335>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_vertex_colorable>>|<pageref|auto-337>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|minimal_edge_coloring>>|<pageref|auto-340>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|chromatic_index>>|<pageref|auto-342>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_eulerian>>|<pageref|auto-346>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|is_hamiltonian>>|<pageref|auto-348>>

      <tuple|<tuple|<with|font-family|<quote|tt>|shortest_path>>|<pageref|auto-351>>

      <tuple|<tuple|<with|font-family|<quote|tt>|dijkstra>>|<pageref|auto-353>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|bellman_ford>>|<pageref|auto-354>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|traveling_salesman>>|<pageref|auto-356>>

      <tuple|<tuple|<with|font-family|<quote|tt>|spanning_tree>>|<pageref|auto-359>>

      <tuple|<tuple|<with|font-family|<quote|tt>|minimal_spanning_tree>>|<pageref|auto-361>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|number_of_spanning_trees>>|<pageref|auto-363>>

      <tuple|<tuple|<with|font-family|<quote|tt>|draw_graph>>|<pageref|auto-366>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|set_vertex_positions>>|<pageref|auto-378>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|highlight_vertex>>|<pageref|auto-382>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|highlight_edges>>|<pageref|auto-384>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|highlight_trail>>|<pageref|auto-385>>

      <tuple|<tuple|<with|font-family|<quote|tt>|language|<quote|verbatim>|highlight_subgraph>>|<pageref|auto-387>>
    </associate>
    <\associate|table>
      <tuple|normal|<surround|<hidden|<tuple>>||interpretation of abstract
      vertex/edge colors in <with|font-family|<quote|ss>|Xcas>>|<pageref|auto-329>>
    </associate>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|Introduction>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-1><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|1.<space|2spc>Constructing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-2><vspace|0.5fn>

      1.1.<space|2spc>General graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3>

      <with|par-left|<quote|1tab>|1.1.1.<space|2spc>Undirected graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-4>>

      <with|par-left|<quote|1tab>|1.1.2.<space|2spc>Directed graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-6>>

      <with|par-left|<quote|1tab>|1.1.3.<space|2spc>Examples
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <with|par-left|<quote|4tab>|Creating vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9><vspace|0.15fn>>

      <with|par-left|<quote|4tab>|Creating edges and arcs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-10><vspace|0.15fn>>

      <with|par-left|<quote|4tab>|Creating paths and trails
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-11><vspace|0.15fn>>

      <with|par-left|<quote|4tab>|Specifying adjacency or weight matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-12><vspace|0.15fn>>

      <with|par-left|<quote|4tab>|Creating special graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-13><vspace|0.15fn>>

      1.2.<space|2spc>Cycle and path graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-14>

      <with|par-left|<quote|1tab>|1.2.1.<space|2spc>Cycle graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-15>>

      <with|par-left|<quote|1tab>|1.2.2.<space|2spc>Path graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-17>>

      <with|par-left|<quote|1tab>|1.2.3.<space|2spc>Trails of edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-19>>

      1.3.<space|2spc>Complete graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-22>

      <with|par-left|<quote|1tab>|1.3.1.<space|2spc>Complete (multipartite)
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-23>>

      <with|par-left|<quote|1tab>|1.3.2.<space|2spc>Complete trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-25>>

      1.4.<space|2spc>Sequence graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-28>

      <with|par-left|<quote|1tab>|1.4.1.<space|2spc>Creating graphs from
      degree sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-29>>

      <with|par-left|<quote|1tab>|1.4.2.<space|2spc>Validating graphic
      sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-31>>

      1.5.<space|2spc>Intersection graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-33>

      <with|par-left|<quote|1tab>|1.5.1.<space|2spc>Interval graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-34>>

      <with|par-left|<quote|1tab>|1.5.2.<space|2spc>Kneser graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-36>>

      1.6.<space|2spc>Special graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-39>

      <with|par-left|<quote|1tab>|1.6.1.<space|2spc>Hypercube graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-40>>

      <with|par-left|<quote|1tab>|1.6.2.<space|2spc>Star graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-42>>

      <with|par-left|<quote|1tab>|1.6.3.<space|2spc>Wheel graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-44>>

      <with|par-left|<quote|1tab>|1.6.4.<space|2spc>Web graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-46>>

      <with|par-left|<quote|1tab>|1.6.5.<space|2spc>Prism graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-48>>

      <with|par-left|<quote|1tab>|1.6.6.<space|2spc>Antiprism graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-50>>

      <with|par-left|<quote|1tab>|1.6.7.<space|2spc>Grid graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-52>>

      <with|par-left|<quote|1tab>|1.6.8.<space|2spc>Sierpi«ski graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-55>>

      <with|par-left|<quote|1tab>|1.6.9.<space|2spc>Generalized Petersen
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-57>>

      <with|par-left|<quote|1tab>|1.6.10.<space|2spc><rigid|LCF> graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-59>>

      1.7.<space|2spc>Isomorphic copies of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-61>

      <with|par-left|<quote|1tab>|1.7.1.<space|2spc>Creating isomorphic
      copies from permutations <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-62>>

      <with|par-left|<quote|1tab>|1.7.2.<space|2spc>Permuting vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-64>>

      <with|par-left|<quote|1tab>|1.7.3.<space|2spc>Relabeling vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-66>>

      1.8.<space|2spc>Subgraphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-68>

      <with|par-left|<quote|1tab>|1.8.1.<space|2spc>Extracting subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-69>>

      <with|par-left|<quote|1tab>|1.8.2.<space|2spc>Induced subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-71>>

      <with|par-left|<quote|1tab>|1.8.3.<space|2spc>Underlying graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-73>>

      <with|par-left|<quote|1tab>|1.8.4.<space|2spc>Fundamental cycles
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-75>>

      1.9.<space|2spc>Operations on graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-78>

      <with|par-left|<quote|1tab>|1.9.1.<space|2spc>Graph complement
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-79>>

      <with|par-left|<quote|1tab>|1.9.2.<space|2spc>Seidel switching
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-81>>

      <with|par-left|<quote|1tab>|1.9.3.<space|2spc>Transposing graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-83>>

      <with|par-left|<quote|1tab>|1.9.4.<space|2spc>Union of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-85>>

      <with|par-left|<quote|1tab>|1.9.5.<space|2spc>Disjoint union of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-87>>

      <with|par-left|<quote|1tab>|1.9.6.<space|2spc>Joining two graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-89>>

      <with|par-left|<quote|1tab>|1.9.7.<space|2spc>Power graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-91>>

      <with|par-left|<quote|1tab>|1.9.8.<space|2spc>Graph products
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-93>>

      <with|par-left|<quote|1tab>|1.9.9.<space|2spc>Transitive closure graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-96>>

      <with|par-left|<quote|1tab>|1.9.10.<space|2spc>Line graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-98>>

      <with|par-left|<quote|1tab>|1.9.11.<space|2spc>Plane dual graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-100>>

      <with|par-left|<quote|1tab>|1.9.12.<space|2spc>Truncating planar graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-103>>

      1.10.<space|2spc>Random graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-105>

      <with|par-left|<quote|1tab>|1.10.1.<space|2spc>Random general graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-106>>

      <with|par-left|<quote|1tab>|1.10.2.<space|2spc>Random bipartite graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-109>>

      <with|par-left|<quote|1tab>|1.10.3.<space|2spc>Random trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-111>>

      <with|par-left|<quote|1tab>|1.10.4.<space|2spc>Random planar graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-113>>

      <with|par-left|<quote|1tab>|1.10.5.<space|2spc>Random graphs from a
      given degree sequence <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-115>>

      <with|par-left|<quote|1tab>|1.10.6.<space|2spc>Random regular graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-117>>

      <with|par-left|<quote|1tab>|1.10.7.<space|2spc>Random tournaments
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-119>>

      <with|par-left|<quote|1tab>|1.10.8.<space|2spc>Random network graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-121>>

      <with|par-left|<quote|1tab>|1.10.9.<space|2spc>Randomizing edge weights
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-123>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|2.<space|2spc>Modifying
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-125><vspace|0.5fn>

      2.1.<space|2spc>Promoting to directed and weighted graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-126>

      <with|par-left|<quote|1tab>|2.1.1.<space|2spc>Converting edges to arcs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-127>>

      <with|par-left|<quote|1tab>|2.1.2.<space|2spc>Assigning weight matrix
      to unweighted graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-129>>

      2.2.<space|2spc>Modifying vertices of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-131>

      <with|par-left|<quote|1tab>|2.2.1.<space|2spc>Adding and removing
      vertices <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-132>>

      2.3.<space|2spc>Modifying edges of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-135>

      <with|par-left|<quote|1tab>|2.3.1.<space|2spc>Adding and removing edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-136>>

      <with|par-left|<quote|1tab>|2.3.2.<space|2spc>Accessing and modifying
      edge weights <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-141>>

      <with|par-left|<quote|1tab>|2.3.3.<space|2spc>Contracting edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-144>>

      <with|par-left|<quote|1tab>|2.3.4.<space|2spc>Subdividing edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-146>>

      2.4.<space|2spc>Using attributes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-148>

      <with|par-left|<quote|1tab>|2.4.1.<space|2spc>Graph attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-149>>

      <with|par-left|<quote|1tab>|2.4.2.<space|2spc>Vertex attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-154>>

      <with|par-left|<quote|1tab>|2.4.3.<space|2spc>Edge attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-159>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|3.<space|2spc>Import
      and export> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-164><vspace|0.5fn>

      3.1.<space|2spc>Importing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-165>

      <with|par-left|<quote|1tab>|3.1.1.<space|2spc>Loading graphs from
      <with|font-family|<quote|ss>|dot> files
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-166>>

      <with|par-left|<quote|1tab>|3.1.2.<space|2spc>The
      <with|font-family|<quote|ss>|dot> file format overview
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-168>>

      3.2.<space|2spc>Exporting graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-169>

      <with|par-left|<quote|1tab>|3.2.1.<space|2spc>Saving graphs in
      <with|font-family|<quote|ss>|dot> format
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-170>>

      <with|par-left|<quote|1tab>|3.2.2.<space|2spc>Saving graph drawings in
      L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|<quote|0>|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
      format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-172>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|4.<space|2spc>Graph
      properties> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-174><vspace|0.5fn>

      4.1.<space|2spc>Basic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-175>

      <with|par-left|<quote|1tab>|4.1.1.<space|2spc>Determining the type of a
      graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-176>>

      <with|par-left|<quote|1tab>|4.1.2.<space|2spc>Listing vertices and
      edges <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-179>>

      <with|par-left|<quote|1tab>|4.1.3.<space|2spc>Equality of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-185>>

      <with|par-left|<quote|1tab>|4.1.4.<space|2spc>Vertex degrees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-187>>

      <with|par-left|<quote|1tab>|4.1.5.<space|2spc>Regular graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-194>>

      <with|par-left|<quote|1tab>|4.1.6.<space|2spc>Strongly regular graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-196>>

      <with|par-left|<quote|1tab>|4.1.7.<space|2spc>Vertex adjacency
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-198>>

      <with|par-left|<quote|1tab>|4.1.8.<space|2spc>Tournament graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-204>>

      <with|par-left|<quote|1tab>|4.1.9.<space|2spc>Bipartite graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-206>>

      <with|par-left|<quote|1tab>|4.1.10.<space|2spc>Edge incidence
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-208>>

      4.2.<space|2spc>Algebraic properties
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-210>

      <with|par-left|<quote|1tab>|4.2.1.<space|2spc>Adjacency matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-211>>

      <with|par-left|<quote|1tab>|4.2.2.<space|2spc>Laplacian matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-213>>

      <with|par-left|<quote|1tab>|4.2.3.<space|2spc>Incidence matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-215>>

      <with|par-left|<quote|1tab>|4.2.4.<space|2spc>Weight matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-217>>

      <with|par-left|<quote|1tab>|4.2.5.<space|2spc>Characteristic polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-219>>

      <with|par-left|<quote|1tab>|4.2.6.<space|2spc>Graph spectrum
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-221>>

      <with|par-left|<quote|1tab>|4.2.7.<space|2spc>Seidel spectrum
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-223>>

      <with|par-left|<quote|1tab>|4.2.8.<space|2spc>Integer graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-225>>

      4.3.<space|2spc>Graph isomorphism <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-227>

      <with|par-left|<quote|1tab>|4.3.1.<space|2spc>Isomorphic graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-228>>

      <with|par-left|<quote|1tab>|4.3.2.<space|2spc>Canonical labeling
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-230>>

      <with|par-left|<quote|1tab>|4.3.3.<space|2spc>Graph automorphisms
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-232>>

      4.4.<space|2spc>Graph polynomials <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-234>

      <with|par-left|<quote|1tab>|4.4.1.<space|2spc>Tutte polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-235>>

      <with|par-left|<quote|1tab>|4.4.2.<space|2spc>Chromatic polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-237>>

      <with|par-left|<quote|1tab>|4.4.3.<space|2spc>Flow polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-239>>

      <with|par-left|<quote|1tab>|4.4.4.<space|2spc>Reliability polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-241>>

      4.5.<space|2spc>Connectivity <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-243>

      <with|par-left|<quote|1tab>|4.5.1.<space|2spc>Connected, biconnected
      and triconnected graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-244>>

      <with|par-left|<quote|1tab>|4.5.2.<space|2spc>Connected and biconnected
      components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-248>>

      <with|par-left|<quote|1tab>|4.5.3.<space|2spc>Vertex connectivity
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-251>>

      <with|par-left|<quote|1tab>|4.5.4.<space|2spc>Graph rank
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-253>>

      <with|par-left|<quote|1tab>|4.5.5.<space|2spc>Articulation points
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-255>>

      <with|par-left|<quote|1tab>|4.5.6.<space|2spc>Strongly connected
      components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-257>>

      <with|par-left|<quote|1tab>|4.5.7.<space|2spc>Edge connectivity
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-260>>

      <with|par-left|<quote|1tab>|4.5.8.<space|2spc>Edge cuts
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-262>>

      <with|par-left|<quote|1tab>|4.5.9.<space|2spc>Two-edge-connected graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-264>>

      4.6.<space|2spc>Trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-267>

      <with|par-left|<quote|1tab>|4.6.1.<space|2spc>Tree graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-268>>

      <with|par-left|<quote|1tab>|4.6.2.<space|2spc>Forest graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-270>>

      <with|par-left|<quote|1tab>|4.6.3.<space|2spc>Height of a tree
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-272>>

      <with|par-left|<quote|1tab>|4.6.4.<space|2spc>Lowest common ancestor of
      a pair of nodes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-274>>

      <with|par-left|<quote|1tab>|4.6.5.<space|2spc>Arborescence graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-276>>

      4.7.<space|2spc>Networks <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-278>

      <with|par-left|<quote|1tab>|4.7.1.<space|2spc>Network graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-279>>

      <with|par-left|<quote|1tab>|4.7.2.<space|2spc>Maximum flow
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-281>>

      <with|par-left|<quote|1tab>|4.7.3.<space|2spc>Minimum cut
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-283>>

      4.8.<space|2spc>Distance in graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-285>

      <with|par-left|<quote|1tab>|4.8.1.<space|2spc>Vertex distance
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-286>>

      <with|par-left|<quote|1tab>|4.8.2.<space|2spc>All-pairs vertex distance
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-288>>

      <with|par-left|<quote|1tab>|4.8.3.<space|2spc>Diameter
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-290>>

      <with|par-left|<quote|1tab>|4.8.4.<space|2spc>Girth
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-291>>

      4.9.<space|2spc>Acyclic graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-294>

      <with|par-left|<quote|1tab>|4.9.1.<space|2spc>Acyclic graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-295>>

      <with|par-left|<quote|1tab>|4.9.2.<space|2spc>Topological sorting
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-297>>

      <with|par-left|<quote|1tab>|4.9.3.<space|2spc><with|font-family|<quote|ss>|st>
      ordering <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-300>>

      4.10.<space|2spc>Matching in graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-302>

      <with|par-left|<quote|1tab>|4.10.1.<space|2spc>Maximum matching
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-303>>

      <with|par-left|<quote|1tab>|4.10.2.<space|2spc>Maximum matching in
      bipartite graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-305>>

      4.11.<space|2spc>Cliques <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-307>

      <with|par-left|<quote|1tab>|4.11.1.<space|2spc>Clique graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-308>>

      <with|par-left|<quote|1tab>|4.11.2.<space|2spc>Maximal cliques
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-310>>

      <with|par-left|<quote|1tab>|4.11.3.<space|2spc>Maximum clique
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-312>>

      <with|par-left|<quote|1tab>|4.11.4.<space|2spc>Minimum clique cover
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-315>>

      <with|par-left|<quote|1tab>|4.11.5.<space|2spc>Clique cover number
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-317>>

      4.12.<space|2spc>Triangles in graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-319>

      <with|par-left|<quote|1tab>|4.12.1.<space|2spc>Counting triangles
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-320>>

      <with|par-left|<quote|1tab>|4.12.2.<space|2spc>Clustering coefficient
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-322>>

      <with|par-left|<quote|1tab>|4.12.3.<space|2spc>Network transitivity
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-324>>

      4.13.<space|2spc>Vertex coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-326>

      <with|par-left|<quote|1tab>|4.13.1.<space|2spc>Greedy vertex coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-327>>

      <with|par-left|<quote|1tab>|4.13.2.<space|2spc>Minimal vertex coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-330>>

      <with|par-left|<quote|1tab>|4.13.3.<space|2spc>Chromatic number
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-332>>

      <with|par-left|<quote|1tab>|4.13.4.<space|2spc>Mycielski graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-334>>

      <with|par-left|<quote|1tab>|4.13.5.<space|2spc><with|mode|<quote|math>|k>-coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-336>>

      4.14.<space|2spc>Edge coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-338>

      <with|par-left|<quote|1tab>|4.14.1.<space|2spc>Minimal edge coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-339>>

      <with|par-left|<quote|1tab>|4.14.2.<space|2spc>Chromatic index
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-341>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|5.<space|2spc>Traversing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-343><vspace|0.5fn>

      5.1.<space|2spc>Walks and tours <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-344>

      <with|par-left|<quote|1tab>|5.1.1.<space|2spc>Eulerian graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-345>>

      <with|par-left|<quote|1tab>|5.1.2.<space|2spc>Hamiltonian graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-347>>

      5.2.<space|2spc>Optimal routing <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-349>

      <with|par-left|<quote|1tab>|5.2.1.<space|2spc>Shortest unweighted paths
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-350>>

      <with|par-left|<quote|1tab>|5.2.2.<space|2spc>Cheapest weighted paths
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-352>>

      <with|par-left|<quote|1tab>|5.2.3.<space|2spc>Traveling salesman
      problem <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-355>>

      5.3.<space|2spc>Spanning trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-357>

      <with|par-left|<quote|1tab>|5.3.1.<space|2spc>Construction of spanning
      trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-358>>

      <with|par-left|<quote|1tab>|5.3.2.<space|2spc>Minimal spanning tree
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-360>>

      <with|par-left|<quote|1tab>|5.3.3.<space|2spc>Counting the spanning
      trees in a graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-362>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|6.<space|2spc>Visualizing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-364><vspace|0.5fn>

      6.1.<space|2spc>Drawing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-365>

      <with|par-left|<quote|1tab>|6.1.1.<space|2spc>Overview
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-367>>

      <with|par-left|<quote|1tab>|6.1.2.<space|2spc>Drawing disconnected
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-368>>

      <with|par-left|<quote|1tab>|6.1.3.<space|2spc>Spring method
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-369>>

      <with|par-left|<quote|1tab>|6.1.4.<space|2spc>Drawing trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-370>>

      <with|par-left|<quote|1tab>|6.1.5.<space|2spc>Drawing planar graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-371>>

      <with|par-left|<quote|1tab>|6.1.6.<space|2spc>Circular graph drawings
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-375>>

      6.2.<space|2spc>Vertex positions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-376>

      <with|par-left|<quote|1tab>|6.2.1.<space|2spc>Setting vertex positions
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-377>>

      <with|par-left|<quote|1tab>|6.2.2.<space|2spc>Generating vertex
      positions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-379>>

      6.3.<space|2spc>Highlighting parts of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-380>

      <with|par-left|<quote|1tab>|6.3.1.<space|2spc>Highlighting vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-381>>

      <with|par-left|<quote|1tab>|6.3.2.<space|2spc>Highlighting edges and
      trails <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-383>>

      <with|par-left|<quote|1tab>|6.3.3.<space|2spc>Highlighting subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-386>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|Bibliography>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-388><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|Command
      Index> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-389><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>