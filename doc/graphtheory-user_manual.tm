<TeXmacs|1.99.6>

<style|<tuple|mmxmanual|giac>>

<\body>
  <\hide-preamble>
    \;

    <assign|figure-text|<macro|<localize|Fig.>>>
  </hide-preamble>

  <\doc-data|<doc-title|Graph theory package for
  Giac/Xcas>|<doc-subtitle|Reference manual>|<\doc-date>
    June 2018
  </doc-date>|<doc-misc|draft>||<doc-running-author|Luka Marohni¢>>
    \;
  <|doc-data>
    \;
  </doc-data>

  <\table-of-contents|toc>
    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|1.<space|2spc>Introduction>
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-1><vspace|0.5fn>

    1.1.<space|2spc>Overview <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-2>

    1.2.<space|2spc>Notations used in this manual
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-3>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|2.<space|2spc>Constructing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-4><vspace|0.5fn>

    2.1.<space|2spc>General graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-5>

    <with|par-left|1tab|2.1.1.<space|2spc>Creating undirected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-6>>

    <with|par-left|1tab|2.1.2.<space|2spc>Creating directed graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-7>>

    <with|par-left|1tab|2.1.3.<space|2spc>Creating vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-8>>

    <with|par-left|1tab|2.1.4.<space|2spc>Creating edges and arcs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-9>>

    <with|par-left|1tab|2.1.5.<space|2spc>Creating paths and trails
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-10>>

    <with|par-left|1tab|2.1.6.<space|2spc>Specifying adjacency or weight
    matrix <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-11>>

    2.2.<space|2spc>Promoting to directed and undirected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-12>

    <with|par-left|1tab|2.2.1.<space|2spc>Converting edges to pairs of arcs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-13>>

    <with|par-left|1tab|2.2.2.<space|2spc>Assigning weight matrix to an
    unweighted graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-14>>

    2.3.<space|2spc>Cycle and path graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-15>

    <with|par-left|1tab|2.3.1.<space|2spc>Cycle graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-16>>

    <with|par-left|1tab|2.3.2.<space|2spc>Path graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-17>>

    <with|par-left|1tab|2.3.3.<space|2spc>Trails of edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-18>>

    2.4.<space|2spc>Complete graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-19>

    <with|par-left|1tab|2.4.1.<space|2spc>Complete graphs with multiple
    vertex partitions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-20>>

    <with|par-left|1tab|2.4.2.<space|2spc>Complete trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-21>>

    2.5.<space|2spc>Sequence graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-22>

    <with|par-left|1tab|2.5.1.<space|2spc>Creating graphs from degree
    sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-23>>

    <with|par-left|1tab|2.5.2.<space|2spc>Validating graphic sequences
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-24>>

    2.6.<space|2spc>Intersection graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-25>

    <with|par-left|1tab|2.6.1.<space|2spc>Interval graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-26>>

    <with|par-left|1tab|2.6.2.<space|2spc>Kneser graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-27>>

    2.7.<space|2spc>Special graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-28>

    <with|par-left|1tab|2.7.1.<space|2spc>Hypercube graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-29>>

    <with|par-left|1tab|2.7.2.<space|2spc>Star graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-30>>

    <with|par-left|1tab|2.7.3.<space|2spc>Wheel graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-31>>

    <with|par-left|1tab|2.7.4.<space|2spc>Web graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-32>>

    <with|par-left|1tab|2.7.5.<space|2spc>Prism graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-33>>

    <with|par-left|1tab|2.7.6.<space|2spc>Antiprism graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-34>>

    <with|par-left|1tab|2.7.7.<space|2spc>Grid graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-35>>

    <with|par-left|1tab|2.7.8.<space|2spc>Sierpi«ski graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-36>>

    <with|par-left|1tab|2.7.9.<space|2spc>Generalized Petersen graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-37>>

    <with|par-left|1tab|2.7.10.<space|2spc><rigid|LCF> graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-38>>

    2.8.<space|2spc>Isomorphic copies of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-39>

    <with|par-left|1tab|2.8.1.<space|2spc>Creating an isomorphic copy of a
    graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-40>>

    <with|par-left|1tab|2.8.2.<space|2spc>Permuting graph vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-41>>

    <with|par-left|1tab|2.8.3.<space|2spc>Relabeling graph vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-42>>

    2.9.<space|2spc>Subgraphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-43>

    <with|par-left|1tab|2.9.1.<space|2spc>Extracting subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-44>>

    <with|par-left|1tab|2.9.2.<space|2spc>Induced subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-45>>

    <with|par-left|1tab|2.9.3.<space|2spc>Underlying graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-46>>

    2.10.<space|2spc>Operations on graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-47>

    <with|par-left|1tab|2.10.1.<space|2spc>Graph complement
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-48>>

    <with|par-left|1tab|2.10.2.<space|2spc>Seidel switching
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-49>>

    <with|par-left|1tab|2.10.3.<space|2spc>Transposing graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-50>>

    <with|par-left|1tab|2.10.4.<space|2spc>Union of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-51>>

    <with|par-left|1tab|2.10.5.<space|2spc>Disjoint union of graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-52>>

    <with|par-left|1tab|2.10.6.<space|2spc>Joining two graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-53>>

    <with|par-left|1tab|2.10.7.<space|2spc>Power graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-54>>

    <with|par-left|1tab|2.10.8.<space|2spc>Graph products
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-55>>

    <with|par-left|1tab|2.10.9.<space|2spc>Transitive closure graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-56>>

    <with|par-left|1tab|2.10.10.<space|2spc>Line graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-57>>

    <with|par-left|1tab|2.10.11.<space|2spc>Plane dual graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-58>>

    2.11.<space|2spc>Random graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-59>

    <with|par-left|1tab|2.11.1.<space|2spc>Random general graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-60>>

    <with|par-left|1tab|2.11.2.<space|2spc>Random bipartite graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-61>>

    <with|par-left|1tab|2.11.3.<space|2spc>Random trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-62>>

    <with|par-left|1tab|2.11.4.<space|2spc>Random planar graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-63>>

    <with|par-left|1tab|2.11.5.<space|2spc>Random regular graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-64>>

    <with|par-left|1tab|2.11.6.<space|2spc>Random tournaments
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-65>>

    <with|par-left|1tab|2.11.7.<space|2spc>Random flow networks
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-66>>

    <with|par-left|1tab|2.11.8.<space|2spc>Randomizing edge weights
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-67>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|3.<space|2spc>Modifying
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-68><vspace|0.5fn>

    3.1.<space|2spc>Modifying vertices of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-69>

    <with|par-left|1tab|3.1.1.<space|2spc>Adding and removing single vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-70>>

    3.2.<space|2spc>Modifying edges of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-71>

    <with|par-left|1tab|3.2.1.<space|2spc>Adding and removing single edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-72>>

    <with|par-left|1tab|3.2.2.<space|2spc>Accessing and modifying edge
    weights <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-73>>

    <with|par-left|1tab|3.2.3.<space|2spc>Contracting edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-74>>

    <with|par-left|1tab|3.2.4.<space|2spc>Subdividing edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-75>>

    3.3.<space|2spc>Using attributes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-76>

    <with|par-left|1tab|3.3.1.<space|2spc>Graph attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-77>>

    <with|par-left|1tab|3.3.2.<space|2spc>Vertex attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-78>>

    <with|par-left|1tab|3.3.3.<space|2spc>Edge attributes
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-79>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|4.<space|2spc>Import
    and export> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-80><vspace|0.5fn>

    4.1.<space|2spc>Importing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-81>

    <with|par-left|1tab|4.1.1.<space|2spc>Loading graphs from
    <with|font-family|ss|dot> files <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-82>>

    <with|par-left|1tab|4.1.2.<space|2spc>The <with|font-family|ss|dot> file
    format overview <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-83>>

    4.2.<space|2spc>Exporting graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-84>

    <with|par-left|1tab|4.2.1.<space|2spc>Saving graphs in
    <with|font-family|ss|dot> format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-85>>

    <with|par-left|1tab|4.2.2.<space|2spc>Saving graph drawings in
    L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|0|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
    format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-86>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|5.<space|2spc>Graph
    properties> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-88><vspace|0.5fn>

    5.1.<space|2spc>Basic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-89>

    <with|par-left|1tab|5.1.1.<space|2spc>Listing vertices and edges
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-90>>

    <with|par-left|1tab|5.1.2.<space|2spc>Vertex degrees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-91>>

    <with|par-left|1tab|5.1.3.<space|2spc>Vertex adjacency
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-92>>

    <with|par-left|1tab|5.1.4.<space|2spc>Edge incidence
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-93>>

    5.2.<space|2spc>Algebraic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-94>

    <with|par-left|1tab|5.2.1.<space|2spc>Adjacency matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-95>>

    <with|par-left|1tab|5.2.2.<space|2spc>Weight matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-96>>

    <with|par-left|1tab|5.2.3.<space|2spc>Incidence matrix
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-97>>

    <with|par-left|1tab|5.2.4.<space|2spc>Characteristic polynomial
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-98>>

    <with|par-left|1tab|5.2.5.<space|2spc>Graph spectrum
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-99>>

    <with|par-left|1tab|5.2.6.<space|2spc>Seidel spectrum
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-100>>

    <with|par-left|1tab|5.2.7.<space|2spc>Integer graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-101>>

    5.3.<space|2spc>Connectivity <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-102>

    <with|par-left|1tab|5.3.1.<space|2spc>Vertex connectivity
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-103>>

    <with|par-left|1tab|5.3.2.<space|2spc>Connected and biconnected
    components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-104>>

    <with|par-left|1tab|5.3.3.<space|2spc>Graph rank
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-105>>

    <with|par-left|1tab|5.3.4.<space|2spc>Articulation points
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-106>>

    <with|par-left|1tab|5.3.5.<space|2spc>Strongly connected components
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-107>>

    <with|par-left|1tab|5.3.6.<space|2spc>Edge connectivity
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-108>>

    5.4.<space|2spc>Trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-109>

    <with|par-left|1tab|5.4.1.<space|2spc>Tree graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-110>>

    <with|par-left|1tab|5.4.2.<space|2spc>Forest graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-111>>

    <with|par-left|1tab|5.4.3.<space|2spc>Height of a tree
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-112>>

    <with|par-left|1tab|5.4.4.<space|2spc>Lowest common ancestor of a pair of
    nodes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-113>>

    5.5.<space|2spc>Distance <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-114>

    <with|par-left|1tab|5.5.1.<space|2spc>Vertex distance
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-115>>

    <with|par-left|1tab|5.5.2.<space|2spc>All-pairs vertex distance
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-116>>

    <with|par-left|1tab|5.5.3.<space|2spc>Diameter of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-117>>

    <with|par-left|1tab|5.5.4.<space|2spc>Girth of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-118>>

    5.6.<space|2spc>Acyclic graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-119>

    <with|par-left|1tab|5.6.1.<space|2spc>Checking if a graph is acyclic
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-120>>

    <with|par-left|1tab|5.6.2.<space|2spc>Topological sorting
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-121>>

    <with|par-left|1tab|5.6.3.<space|2spc><with|font-family|ss|st> ordering
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-122>>

    5.7.<space|2spc>Vertex matching <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-123>

    <with|par-left|1tab|5.7.1.<space|2spc>Maximum matching
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-124>>

    <with|par-left|1tab|5.7.2.<space|2spc>Maximum matching in bipartite
    graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-125>>

    5.8.<space|2spc>Cliques <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-126>

    <with|par-left|1tab|5.8.1.<space|2spc>Clique graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-127>>

    <with|par-left|1tab|5.8.2.<space|2spc>Triangle-free graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-128>>

    <with|par-left|1tab|5.8.3.<space|2spc>Maximal cliques
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-129>>

    <with|par-left|1tab|5.8.4.<space|2spc>Maximum clique
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-130>>

    <with|par-left|1tab|5.8.5.<space|2spc>Minimum clique cover
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-131>>

    5.9.<space|2spc>Vertex coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-132>

    <with|par-left|1tab|5.9.1.<space|2spc>Greedy coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-133>>

    <with|par-left|1tab|5.9.2.<space|2spc>Minimal coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-135>>

    <with|par-left|1tab|5.9.3.<space|2spc>Chromatic number
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-137>>

    <with|par-left|1tab|5.9.4.<space|2spc><with|mode|math|k>-coloring
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-138>>

    5.10.<space|2spc>Edge coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-139>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|6.<space|2spc>Traversing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-140><vspace|0.5fn>

    6.1.<space|2spc>Walks and tours <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-141>

    <with|par-left|1tab|6.1.1.<space|2spc>Eulerian graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-142>>

    <with|par-left|1tab|6.1.2.<space|2spc>Hamiltonian graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-143>>

    6.2.<space|2spc>Optimal routing <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-144>

    <with|par-left|1tab|6.2.1.<space|2spc>Shortest paths in unweighted graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-145>>

    <with|par-left|1tab|6.2.2.<space|2spc>Cheapest paths in weighted graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-146>>

    <with|par-left|1tab|6.2.3.<space|2spc>Traveling salesman problem
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-147>>

    6.3.<space|2spc>Spanning trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-148>

    <with|par-left|1tab|6.3.1.<space|2spc>Constructing a spanning tree
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-149>>

    <with|par-left|1tab|6.3.2.<space|2spc>Minimal spanning tree
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-150>>

    <with|par-left|1tab|6.3.3.<space|2spc>Counting all spanning trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-151>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|7.<space|2spc>Visualizing
    graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-152><vspace|0.5fn>

    7.1.<space|2spc>Drawing graphs using various methods
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-153>

    <with|par-left|1tab|7.1.1.<space|2spc>Overview
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-154>>

    <with|par-left|1tab|7.1.2.<space|2spc>Drawing disconnected graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-155>>

    <with|par-left|1tab|7.1.3.<space|2spc>Spring method
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-156>>

    <with|par-left|1tab|7.1.4.<space|2spc>Drawing trees
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-157>>

    <with|par-left|1tab|7.1.5.<space|2spc>Drawing planar graphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-158>>

    <with|par-left|1tab|7.1.6.<space|2spc>Circular graph drawings
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-162>>

    7.2.<space|2spc>Custom vertex positions
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-163>

    <with|par-left|1tab|7.2.1.<space|2spc>Setting vertex positions
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-164>>

    <with|par-left|1tab|7.2.2.<space|2spc>Generating vertex positions
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-165>>

    7.3.<space|2spc>Highlighting parts of a graph
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-166>

    <with|par-left|1tab|7.3.1.<space|2spc>Highlighting vertices
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-167>>

    <with|par-left|1tab|7.3.2.<space|2spc>Highlighting edges and trails
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-168>>

    <with|par-left|1tab|7.3.3.<space|2spc>Highlighting subgraphs
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <no-break><pageref|auto-169>>

    <vspace*|1fn><with|font-series|bold|math-font-series|bold|font-shape|small-caps|Bibliography>
    <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
    <pageref|auto-170><vspace|0.5fn>
  </table-of-contents>

  <chapter|Introduction>

  <section|Overview>

  This document contains an overview of the graph theory commands built in
  the Giac/Xcas software, including the syntax, the detailed description and
  practical examples for each command.

  <section|Notations used in this manual>

  \;

  <chapter|Constructing graphs>

  <section|General graphs>

  The commands <verbatim|graph> and <verbatim|digraph> are used for
  constructing general graphs.

  <subsection|Creating undirected graphs>

  The command <verbatim|graph> accepts between one and three mandatory
  arguments, each of them being one of the following structural elements of
  the resulting graph:

  <\itemize-minus>
    <item>the number or list of vertices (a vertex may be any atomic object,
    such as an integer, a symbol or a string); it must be the first argument
    if used,

    <item>the set of edges (each edge is a list containing two vertices), a
    permutation, a trail of edges or a sequence of trails; it can be either
    the first or the second argument if used,

    <item>the adjacency or weight matrix.
  </itemize-minus>

  Additionally, some of the following options may be appended to the sequence
  of arguments:

  <\itemize>
    <item><verbatim|directed = true> or <verbatim|false>,

    <item><verbatim|weighted = true> or <verbatim|false>,

    <item><verbatim|color = >an integer or a list of integers representing
    color(s) of the vertices,

    <item><verbatim|coordinates = >a list of vertex <abbr|2D> or <abbr|3D>
    coordinates.
  </itemize>

  The <with|font-family|tt|graph> command may also be called by passing a
  string, representing the name of a special graph, as its only argument. In
  that case the corresponding graph will be constructed and returned. The
  supported graphs and their names are listed below.

  <\enumerate>
    <item>Clebsch graph: <with|font-family|tt|clebsch>

    <item>Coxeter graph: <with|font-family|tt|coxeter>

    <item>Desargues graph: <with|font-family|tt|desargues>

    <item>Dodecahedral graph: <with|font-family|tt|dodecahedron>

    <item>Dürer graph: <with|font-family|tt|durer>

    <item>Dyck graph: <with|font-family|tt|dyck>

    <item>Grinberg graph: <with|font-family|tt|grinberg>

    <item>Grotzsch graph: <with|font-family|tt|grotzsch>

    <item>Harries graph: <with|font-family|tt|harries>

    <item>Harries\UWong graph: <with|font-family|tt|harries-wong>

    <item>Heawood graph: <with|font-family|tt|heawood>

    <item>Herschel graph: <with|font-family|tt|herschel>

    <item>Icosahedral graph: <with|font-family|tt|icosahedron>

    <item>Levi graph: <with|font-family|tt|levi>

    <item>Ljubljana graph: <with|font-family|tt|ljubljana>

    <item>McGee graph: <with|font-family|tt|mcgee>

    <item>Möbius\UKantor graph: <with|font-family|tt|mobius-kantor>

    <item>Nauru graph: <with|font-family|tt|nauru>

    <item>Octahedral graph: <with|font-family|tt|octahedron>

    <item>Pappus graph: <with|font-family|tt|pappus>

    <item>Petersen graph: <with|font-family|tt|petersen>

    <item>Robertson graph: <with|font-family|tt|robertson>

    <item>Truncated icosahedral graph: <with|font-family|tt|soccerball>

    <item>Shrikhande graph: <with|font-family|tt|shrikhande>

    <item>Tetrahedral graph: <with|font-family|tt|tehtrahedron>
  </enumerate>

  <subsection|Creating directed graphs>

  The <with|font-family|tt|digraph> command is used for creating directed
  graphs, although it is also possible with the <with|font-family|tt|graph>
  command by specifying the option <with|font-family|tt|directed=true>.
  Actually, calling <with|font-family|tt|digraph> is the same as calling
  <with|font-family|tt|graph> with that option appended to the sequence of
  arguments. However, creating special graphs is not supported by
  <with|font-family|tt|digraph> since they are all undirected. Edges in
  directed graphs are called <em|arcs>. Edges and arcs are different
  structures: an edge is represented by a two-element set containing its
  endpoints, while an arc is represented by the ordered pairs of its
  endpoints.

  The following series of examples demonstrates the various possibilities
  when using <with|font-family|tt|graph> and <with|font-family|tt|digraph>
  commands.

  <subsection|Creating vertices>

  A graph consisting only of vertices and no edges can be created simply by
  providing the number of vertices or the list of vertex labels.

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

  <subsection|Creating edges and arcs>

  Edges/arcs must be specified inside a set so that it can be distinguished
  from a (adjacency or weight) matrix. If only a set of edges/arcs is
  specified, the vertices needed to establish these will be created
  automatically. Note that, when constructing a directed graph, the order of
  the vertices in an arc matters; in undirected graphs it is not meaningful.

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

    <\textput>
      Edge weights may also be specified.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph(%{[[a,b],2],[[b,c],2.3],[[c,a],3/2]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\textput>
      If the graph contains isolated vertices (not connected to any other
      vertex) or a particular order of vertices is desired, the list of
      vertices has to be specified first.
    </textput>

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

  <subsection|Creating paths and trails>

  A directed graph can also be created from a list of <math|n> vertices and a
  permutation of order <math|n>. The resulting graph consists of a single
  directed path with the vertices ordered according to the permutation.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([a,b,c,d],[1,2,3,0])
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\textput>
      Alternatively, one may specify edges as a trail.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      <with|font-family|tt|digraph([a,b,c,d],trail(b,c,d,a))>
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 4 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\textput>
      Using trails is also possible when creating undirected graphs. Also,
      some vertices in a trail may be repeated, which is not allowed in a
      path.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([a,b,c,d],trail(b,c,d,a,c))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\textput>
      There is also the possibility of specifying several trails in a
      sequence, which is useful for designing more complex graphs.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph(trail(1,2,3,4,2),trail(3,5,6,7,5,4))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 9 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Specifying adjacency or weight matrix>

  A graph can be created from a single square matrix
  <math|A=<around|[|a<rsub|i*j>|]><rsub|n>> of order <math|n>. If it contains
  only ones and zeros and has zeros on its diagonal, it is assumed to be the
  adjacency matrix for the desired graph. Otherwise, if an element outside
  the set <math|<around|{|0,1|}>> is encountered, it is assumed that the
  matrix of edge weights is passed as input, causing the resulting graph to
  be weighted accordingly. In each case, exactly <math|n> vertices will be
  created and <math|i>-th and <math|j>-th vertex will be connected iff
  <math|a<rsub|i*j>\<neq\>0>. If the matrix is symmetric, the resulting graph
  will be undirected, otherwise it will be directed.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([[0,1,1,0],[1,0,0,1],[1,0,0,0],[0,1,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([[0,1.0,2.3,0],[4,0,0,3.1],[0,0,0,0],[0,0,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 4 vertices and 4 arcs>
      </equation*>
    </unfolded-io>

    <\textput>
      List of vertex labels can be specified before the matrix.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph([a,b,c,d],[[0,1,1,0],[1,0,0,1],[1,0,0,0],[0,1,0,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\textput>
      When creating a weighted graph, one can first specify the list of
      <math|n> vertices and the set of edges, followed by a square matrix
      <math|A> of order <math|n>. Then for every edge <math|<around|{|i,j|}>>
      or arc <math|<around|(|i,j|)>> the element <math|a<rsub|i*j>> of
      <math|A> is assigned as its weight. Other elements of <math|A> are
      ignored.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      digraph([a,b,c],%{[a,b],[b,c],[a,c]%}, [[0,1,2],[3,0,4],[5,6,0]])
    <|unfolded-io>
      <\equation*>
        <text|a directed weighted graph with 3 vertices and 3 arcs>
      </equation*>
    </unfolded-io>

    <\textput>
      When a special graph is desired, one just needs to pass its name to the
      <with|font-family|tt|graph> command. An undirected unweighted graph
      will be returned.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Promoting to directed and undirected graphs>

  <subsection|Converting edges to pairs of arcs>

  To promote an existing undirected graph to a directed one or an unweighted
  graph to a weighted one, use the commands <kbd|make_directed> and
  <kbd|make_weighted>, respectively.

  The command <with|font-family|tt|make_directed> is called with one or two
  arguments, an undirected graph <math|G<around|(|V,E|)>> and optionally a
  square matrix of order <math|<around|\||V|\|>>. Every edge
  <math|<around|{|i,j|}>\<in\>E> is replaced with the pair of arcs
  <math|<around|(|i,j|)>> and <math|<around|(|j,i|)>>. If matrix <math|A> is
  specified, <math|a<rsub|i*j>> and <math|a<rsub|j*i>> are assigned as
  weights of these arcs, respectively. Thus a directed (and possibly
  weighted) graph is created and returned.

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
      make_directed(cycle_graph(4), [[0,0,0,1],[2,0,1,3],[0,1,0,4],[5,0,4,0]])
    <|unfolded-io>
      <\equation*>
        <text|C4: a directed weighted graph with 4 vertices and 8 arcs>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Assigning weight matrix to an unweighted graph>

  The command <with|font-family|tt|make_weighted> accepts one or two
  arguments, an unweighted graph <math|G<around|(|V,E|)>> and optionally a
  square matrix <math|A> of order <math|<around|\||V|\|>>. If the matrix
  specification is omitted, a square matrix of ones is assumed. Then a copy
  of <math|G> is returned where each edge/arc <math|<around|(|i,j|)>\<in\>E>
  gets <math|a<rsub|i*j>> assigned as its weight. If <math|G> is an
  undirected graph, it is assumed that <math|A> is symmetric.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      make_weighted(graph(%{[1,2],[2,3],[3,1]%}), [[0,2,3],[2,0,1],[3,1,0]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected weighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Cycle and path graphs>

  <subsection|Cycle graphs>

  Cycle graphs can be created by using the command
  <with|font-family|tt|cycle_graph>.

  <kbd|cycle_graph> accepts a positive integer <math|n> or a list of distinct
  vertices as its only argument and returns the graph consisting of a single
  cycle through the specified vertices in the given order. If <math|n> is
  specified it is assumed to be the desired number of vertices, in which case
  they will be created and labeled with the first <math|n> integers (starting
  from 0 in Xcas mode and from 1 in Maple mode). The resulting graph will be
  given the name <kbd|Cn>, for example <kbd|C4> for <math|n=4>.

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

  <center|<image|images/c5.eps|40%|||>>

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

  Path graphs can be created by using the command
  <with|font-family|tt|path_graph>.

  <kbd|path_graph> accepts a positive integer <math|n> or a list of distinct
  vertices as its only argument and returns a graph consisting of a single
  path through the specified vertices in the given order. If <math|n> is
  specified it is assumed to be the desired number of vertices, in which case
  they will be created and labeled with the first <math|n> integers (starting
  from 0 in <samp|Xcas> mode resp.<nbsp>from 1 in <samp|Maple> mode).

  Note that a path cannot intersect itself. Paths that are allowed to cross
  themselves are called <em|trails> (see the command <kbd|trail>).

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

  <subsection|Trails of edges>

  If the dummy command <with|font-family|tt|trail> is called with a sequence
  of vertices as arguments, it returns the symbolic expression representing
  the trail of edges through the specified vertices. The resulting symbolic
  object is recognizable by <with|font-family|tt|graph> and
  <with|font-family|tt|digraph> commands. Note that a trail may cross itself
  (some vertices may be repeated in the given sequence).

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
  </session>

  <section|Complete graphs>

  <subsection|Complete graphs with multiple vertex partitions>

  To create complete (multipartite) graphs, use the command
  <with|font-family|tt|complete_graph>.

  If <with|font-family|tt|complete_graph> is called with a single argument, a
  positive integer <math|n> or a list of distinct vertices, it returns the
  complete graph with the specified vertices. If integer <math|n> is
  specified, it is assumed that it is the desired number of vertices and they
  will be created and labeled with the first <math|n> integers (starting from
  0 in Xcas mode and from 1 in Maple mode).

  If a sequence of positive integers <math|n<rsub|1>,n<rsub|2>,\<ldots\>,n<rsub|k>>
  is passed as argument, <kbd|complete_graph> returns the complete
  multipartite graph with partitions of size
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

  <center|<image|images/complete.eps|40%|||>>

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

  <center|<image|images/k33.eps|40%|||>>

  <subsection|Complete trees>

  To construct the complete binary tree of depth <math|n>, use the command
  <with|font-family|tt|complete_binary_tree> which accepts <math|n> (a
  positive integer) as its only argument.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      complete_binary_tree(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 6 edges>
      </equation*>
    </unfolded-io>
  </session>

  To construct the complete <math|k>-ary tree of the specified depth use the
  command <with|font-family|tt|complete_kary_tree>.

  <with|font-family|tt|complete_kary_tree> accepts <math|k> and <math|n>
  (positive integers) as its arguments and returns the complete <math|k>-ary
  tree of depth <math|n>. For example, to get a ternary tree with two levels,
  input:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      complete_kary_tree(3,2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 13 vertices and 12 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Sequence graphs>

  <subsection|Creating graphs from degree sequences>

  To construct a graph from its degree sequence, use the command
  <with|font-family|tt|sequence_graph>.

  <kbd|sequence_graph> accepts a list <math|L> of positive integers as its
  only argument and, if <math|L> represents a graphic sequence, the
  corresponding graph <math|G> with <math|<around*|\||L|\|>> <math|vertices>
  is returned. If the argument is not a graphic sequence, an error is
  returned.

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

  The graph <math|G> is constructed in <math|O<around|(|<around|\||L|\|><rsup|2>*log
  <around|\||L|\|>|)>> time by using the algorithm of <name|Havel> and
  <name|Hakimi>.

  <subsection|Validating graphic sequences>

  The command <with|font-family|tt|is_graphic_sequence> is used to check
  whether a list of integers represents the degree sequence of some graph.

  <with|font-family|tt|is_graphic_sequence> accepts a list <math|L> of
  positive integers as its only argument and returns
  <with|font-family|tt|true> if there exists a graph <math|G<around|(|V,E|)>>
  with degree sequence <math|<around|{|deg v:v\<in\>V|}>> equal to <math|L>
  and <with|font-family|tt|false> otherwise. The algorithm, which has the
  complexity <math|O<around|(|<around|\||L|\|><rsup|2>|)>>, is based on the
  theorem of <name|Erd®s> and <name|Gallai>.

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

  The command <with|font-family|tt|interval_graph> is used for creating
  interval graphs.

  <kbd|interval_graph> accepts a sequence or list of real-line intervals as
  its argument and returns an undirected unweighted graph with these
  intervals as vertices (the string representations of the intervals are used
  as labels), each two of them being connected with an edge if and only if
  the corresponding intervals intersect.

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

  <center|<image|images/intersect.png|40%|||>>

  <subsection|Kneser graphs>

  To create Kneser graphs use the commands <with|font-family|tt|kneser_graph>
  and <kbd|odd_graph>.

  <kbd|kneser_graph> accepts two positive integers <math|n\<leq\>20> and
  <math|k> as its arguments and returns the Kneser graph
  <math|K<around*|(|n,k|)>>. The latter is obtained by setting all
  <math|k>-subsets of a set of <math|n> elements as vertices and connecting
  each two of them if and only if the corresponding sets are disjoint.
  Therefore each Kneser graph is the complement of a certain intersection
  graph.

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

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/kneser.eps|40%|||>>

  The command <with|font-family|tt|odd_graph> is used for creating so-called
  <em|odd> graphs, which are Kneser graphs with parameters <math|n=2*d+1> and
  <math|k=d> for <math|d\<geqslant\>1>.

  <with|font-family|tt|odd_graph> accepts a positive integer <math|d\<leq\>8>
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

  The command <verbatim|hypercube_graph> is used for creating hypercube
  graphs.

  <verbatim|hypercube_graph> accepts a positive integer <math|n> as its only
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

  <center|<image|images/cubeplan.eps|40%|||>>

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
  </session>

  <subsection|Star graphs>

  The command <with|font-family|tt|star_graph> is used for creating star
  graphs.

  <with|font-family|tt|star_graph> accepts a positive integer <math|n> as its
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

  <center|<image|images/star.eps|40%|||>>

  <subsection|Wheel graphs>

  The command <with|font-family|tt|wheel_graph> is used for creating wheel
  graphs.

  <with|font-family|tt|wheel_graph> accepts a positive integer <math|n> as
  its only argument and returns the wheel graph with <math|n+1> vertices.

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

  <center|<image|images/wheel.eps|40%|||>>

  <subsection|Web graphs>

  The command <with|font-family|tt|web_graph> is used for creating web
  graphs.

  <with|font-family|tt|web_graph> accepts two positive integers <math|a> and
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

  <center|<image|images/web.eps|40%|||>>

  <subsection|Prism graphs>

  The command <with|font-family|tt|prism_graph> is used for creating prism
  graphs.

  <with|font-family|tt|prism_graph> accepts a positive integer <math|n> as
  its only argument and returns the prism graph with parameter <math|n>,
  namely <with|font-family|tt|petersen_graph(n,1)>.

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

  <center|<image|images/prism.eps|40%|||>>

  <subsection|Antiprism graphs>

  The command <with|font-family|tt|antiprism_graph> is used for creating
  antiprism graphs.

  <with|font-family|tt|antiprism_graph> accepts a positive integer <math|n>
  as its only argument and returns the antiprism graph with parameter
  <math|n>, which is constructed from two concentric cycles of <math|n>
  vertices by joining each vertex of the inner to two adjacent nodes of the
  outer cycle.

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

  <center|<image|images/antiprism.eps|40%|||>>

  <subsection|Grid graphs>

  The command <with|font-family|tt|grid_graph>
  resp.<nbsp><verbatim|torus_grid_graph> is used for creating rectangular
  resp.<nbsp>torus grid graphs.

  <with|font-family|tt|grid_graph> accepts two positive integers <math|m> and
  <math|n> as its arguments and returns the <math|m> by <math|n> grid on
  <math|m\<cdot\>n> vertices, namely the Cartesian product of
  <with|font-family|tt|path_graph(m)> and
  <with|font-family|tt|path_graph(n)>.

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

  <center|<image|images/grid2.eps|40%|||>>

  Connecting vertices in the opposite corners of the generated grid yields an
  interesting new graph.

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

  <center|<image|images/net.eps|50%|||>>

  In the following example, the Möbius strip is constructed by connecting the
  vertices in the oppposite sides of a narrow grid graph.

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

  <center|<image|images/strip.eps|60%|||>>

  \;

  \;

  <with|font-family|tt|torus_grid_graph> accepts two positive integers
  <math|m> and <math|n> as its arguments and returns the <math|m> by <math|n>
  torus grid on <math|m*n> vertices, namely the Cartesian product of
  <with|font-family|tt|cycle_graph(m)> and
  <with|font-family|tt|cycle_graph(n)>.

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

  <center|<image|images/torusgrid.eps|40%|||>>

  <subsection|Sierpi«ski graphs>

  The command <with|font-family|tt|sierpinski_graph> is used for creating
  Sierpi«ski-type graphs <math|S<rsub|k><rsup|n>> and <math|S
  T<rsub|k><rsup|n>> <cite|hinz>.

  <with|font-family|tt|sierpinski_graph> accepts two positive integers
  <math|n> and <math|k> as its arguments (and optionally the symbol
  <with|font-family|tt|triangle> as the third argument) and returns the
  Sierpi«ski (triangle) graph with parameters <math|n> and <math|k>.

  The Sierpi«ski triangle graph <math|S T<rsub|k><rsup|n>> is obtained by
  contracting all non-clique edges in <math|S<rsub|k><rsup|n>>. To detect
  such edges the variant of the algorithm by <name|Bron> and <name|Kerbosch>,
  developed by <name|Tomita> et al.<nbsp>in <cite|tomita>, is used, which can
  be time consuming for <math|n\<gtr\>6>.

  In particular, <math|S T<rsub|3><rsup|n>> is the well-known Sierpi«ski
  sieve<\footnote>
    <hlink|https://en.wikipedia.org/wiki/Sierpinski_triangle|>
  </footnote> graph of order <math|n>.

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

  <center|<image|images/s43.eps|40%|||>>

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

  The command <with|font-family|tt|petersen_graph> is used for creating
  generalized Petersen graphs <math|P<around*|(|n,k|)>>.

  <with|font-family|tt|petersen_graph> accepts two arguments, <math|n> and
  <math|k> (positive integers). The second argument may be omitted, in which
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

  <center|<image|images/petersen.eps|40%|||>>

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

  <center|<image|images/moka.eps|40%|||>>

  Note that Desargues, Dürer and Nauru graphs are also generalized Petersen
  graphs <math|P<around|(|10,3|)>>, <math|P<around|(|6,2|)>> and
  <math|P<around|(|12,5|)>>, respectively.

  <subsection|<abbr|LCF> graphs>

  The command <verbatim|lcf_graph> is used for constructing a cubic
  Hamiltonian graph from its <abbr|LCF> notation<\footnote>
    For the details about <abbr|LCF> notation, see
    <hlink|https://en.wikipedia.org/wiki/LCF_notation|>.
  </footnote>.

  <verbatim|lcf_graph> takes one or two arguments, a list <math|L> of nonzero
  integers, called <em|jumps>, and optionally a positive integer <math|n>,
  called <em|exponent> (by default, <math|n=1>). The command returns the
  graph on <math|n*<around*|\||L|\|>> vertices obtained by iterating the
  sequence of jumps <math|n> times.

  For example, the following command line creates the Frucht graph.

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

  <center|<image|images/frucht.eps|40%|||>>

  The following command line constructs the truncated octahedral graph.

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

  <center|<image|images/oct_trunc.eps|40%|||>>

  <section|Isomorphic copies of graphs>

  The commands <kbd|isomorphic_copy>, <kbd|permute_vertices> and
  <kbd|relabel_vertices> are used for constructing isomorphic copies of
  existing graphs.

  <subsection|Creating an isomorphic copy of a graph>

  The command <with|font-family|tt|isomorphic_copy> accepts two arguments, a
  graph <math|G<around|(|V,E|)>> and a permutation <math|\<sigma\>> of order
  <math|<around|\||V|\|>>, and returns the copy of graph <math|G> with
  vertices rearranged according to <math|\<sigma\>>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      isomorphic_copy(path_graph(5),randperm(5))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 4 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Permuting graph vertices>

  The command <with|font-family|tt|permute_vertices> accepts two arguments, a
  graph <math|G<around|(|V,E|)>> and a list <math|L> of length
  <math|<around|\||V|\|>> containing all vertices from <math|V> in a certain
  order, and returns a copy of <math|G> with vertices rearranged as specified
  by <math|L>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      permute_vertices(path_graph([a,b,c,d]),[b,d,a,c])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Relabeling graph vertices>

  The command <with|font-family|tt|relabel_vertices> accepts two arguments, a
  graph <math|G<around|(|V,E|)>> and a list <math|L> of vertex labels, and
  returns the copy of <math|G> with vertices relabeled with labels from
  <math|L>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      relabel_vertices(path_graph(4),[a,b,c,d])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Subgraphs>

  <subsection|Extracting subgraphs>

  To extract the subgraph of <math|G<around|(|V,E|)>> formed by edges from
  <math|L\<subset\>E>, use the command <with|font-family|tt|subgraph> which
  accepts two arguments: <math|G> and <math|L>.

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

  <center|<image|images/sg0.eps|40%|||>>

  <subsection|Induced subgraphs>

  To obtain the subgraph of <math|G> induced by set of vertices
  <math|L\<subset\>V>, use the command <with|font-family|tt|induced_subgraph>.

  <with|font-family|tt|induced_subgraph> accepts two arguments <math|G> and
  <math|L> and returns the subgraph of <math|G> formed by all edges in
  <math|E> which have endpoints in <math|L>.

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

  <center|<image|images/sg.eps|40%|||>>

  <subsection|Underlying graphs>

  For every graph <math|G<around*|(|V,E|)>> there is an undirected and
  unweighted graph <math|U<around*|(|V,E<rprime|'>|)>>, called the
  <em|underlying graph> of <math|G>, where <math|E<rprime|'>> is obtained
  from <math|E> by dropping edge directions.

  To construct <math|U> use the command <with|font-family|tt|underlying_graph>
  which accepts <math|G> as its only argument. The result is obtained by
  copying <math|G> and \Pforgetting\Q the directions of edges as well as
  their weights.

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

  <section|Operations on graphs>

  <subsection|Graph complement>

  The command <with|font-family|tt|graph_complement> is used for constructing
  complements of graphs.

  <with|font-family|tt|graph_complement> accepts a graph
  <math|G<around|(|V,E|)>> as its only argument and returns the complement
  <math|G<rsup|c><around|(|V,E<rsup|c>|)>> of <math|G>, where
  <math|E<rsup|c>> is the largest set containing only edges/arcs not present
  in <math|G>.

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

  <center|<image|images/c5.eps|40%|||>>

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

  <center|<image|images/star2.eps|40%|||>>

  <subsection|Seidel switching>

  The command <with|font-family|tt|seidel_switch> is used for Seidel
  switching in graphs.

  <with|font-family|tt|seidel_switch> accepts two arguments, an undirected
  and unweighted graph <math|G<around|(|V,E|)>> and a list of vertices
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

  <center|<image|images/seidel.eps|40%|||>>

  <subsection|Transposing graphs>

  The command <with|font-family|tt|reverse_graph> is used for reversing arc
  directions in digraphs.

  <with|font-family|tt|reverse_graph> accepts a graph
  <math|G<around|(|V,E|)>> as its only argument and returns the reverse graph
  <math|G<rsup|T><around|(|V,E<rprime|'>|)>> of <math|G> where
  <math|E<rprime|'>=<around|{|<around|(|j,i|)>:<around|(|i,j|)>\<in\>E|}>>,
  i.e.<nbsp>returns the copy of <math|G> with the directions of all edges
  reversed.

  Note that <with|font-family|tt|reverse_graph> is defined for both directed
  and undirected graphs, but gives meaningful results only for directed
  graphs.

  <math|G<rsup|T>> is also called the <em|transpose graph> of <math|G>
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

  <subsection|Union of graphs>

  The command <with|font-family|tt|graph_union> is used for constructing
  union of two or more graphs.

  <with|font-family|tt|graph_union> accepts a sequence of graphs
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

  To construct disjoint union of graphs use the command
  <with|font-family|tt|disjoint_union>.

  <with|font-family|tt|disjoint_union> accepts a sequence of graphs
  <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the graph obtained by
  labeling all vertices with strings <kbd|k:v> where <math|v\<in\>V<rsub|k>>
  and all edges with strings <kbd|k:e> where <math|e\<in\>E<rsub|k>> and
  calling the <with|font-family|tt|graph_union> command subsequently. As all
  vertices and edges are labeled differently, it follows
  <math|<around|\||V|\|>=<big|sum><rsub|k=1><rsup|n><around|\||V<rsub|k>|\|>>
  and <math|<around|\||E|\|>=<big|sum><rsub|k=1><rsup|n><around|\||E<rsub|k>|\|>>.

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

  <center|<image|images/disjoint.eps|40%|||>>

  <subsection|Joining two graphs>

  The command <with|font-family|tt|graph_join> is used for joining graphs.

  <with|font-family|tt|graph_join> accepts two graphs <math|G> and <math|H>
  as its arguments and returns the graph which is obtained by connecting all
  the vertices of <math|G> to all vertices of <math|H>. The vertex labels in
  the resulting graph are strings of the form <kbd|1:u> and <kbd|2:v> where
  <math|u> is a vertex in <math|G> and <math|v> is a vertex in <math|H>.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph_join(path_graph(2),graph(3))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 7 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/join.eps|40%|||>>

  <subsection|Power graphs>

  The command <with|font-family|tt|graph_power> is used for constructing the
  powers of a graph.

  <with|font-family|tt|graph_power> accepts two arguments, a graph
  <math|G<around|(|V,E|)>> and a positive integer <math|k>, and returns the
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

  <center|<image|images/pow1.eps|40%|||>>

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

  <center|<image|images/pow2.eps|40%|||>>

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

  <center|<image|images/pow3.eps|40%|||>>

  <subsection|Graph products>

  There are two distinct operations for computing the product of two graphs:
  Cartesian product and tensor product. These operations are available in
  Giac as the commands <kbd|cartesian_product> and <kbd|tensor_product>,
  respectively.

  The command <with|font-family|tt|cartesian_product> accepts a sequence of
  graphs <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the Cartesian product
  <math|G<rsub|1>\<times\>G<rsub|2>\<times\>\<cdots\>\<times\>G<rsub|n>> of
  the input graphs. The Cartesian product
  <math|G<around|(|V,E|)>=G<rsub|1>\<times\>G<rsub|2>> is the graph with list
  of vertices <math|V=V<rsub|1>\<times\>V<rsub|2>>, labeled with strings
  <kbd|v1:v2> where <math|v<rsub|1>\<in\>V<rsub|1>> and
  <math|v<rsub|2>\<in\>V<rsub|2>>, such that
  <math|<text|(<kbd|u1:v1>,<kbd|u2:v2>)>\<in\>E> if and only if
  <math|u<rsub|1>> is adjacent to <math|u<rsub|2>> and
  <math|v<rsub|1>=v<rsub|2>> <with|font-series|bold|or>
  <math|u<rsub|1>=u<rsub|2>> and <math|v<rsub|1>> is adjacent to
  <math|v<rsub|2>>.

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

  <center|<image|images/graph4.eps|40%|||>>

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

  <center|<image|images/graph3.eps|40%|||>>

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

  <center|<image|images/prod1.eps|40%|||>>

  The command <with|font-family|tt|tensor_product> accepts a sequence of
  graphs <math|G<rsub|k><around|(|V<rsub|k>,E<rsub|k>|)>> for
  <math|k=1,2,\<ldots\>,n> as its argument and returns the tensor product
  <math|G<rsub|1>\<times\>G<rsub|2>\<times\>\<cdots\>\<times\>G<rsub|n>> of
  the input graphs. The tensor product <math|G<around|(|V,E|)>=G<rsub|1>\<times\>G<rsub|2>>
  is the graph with list of vertices <math|V=V<rsub|1>\<times\>V<rsub|2>>,
  labeled with strings <kbd|v1:v2> where <math|v<rsub|1>\<in\>V<rsub|1>> and
  <math|v<rsub|2>\<in\>V<rsub|2>>, such that
  <math|<text|(<kbd|u1:v1>,<kbd|u2:v2>)>\<in\>E> if and only if
  <math|u<rsub|1>> is adjacent to <math|u<rsub|2>>
  <with|font-series|bold|and> <math|v<rsub|1>> is adjacent to
  <math|v<rsub|2>>.

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

  <center|<image|images/prod2.eps|40%|||>>

  <subsection|Transitive closure graph>

  The command <verbatim|transitive_closure> is used for constructing the
  transitive closure graph of the given graph. Transitive closure of a
  (directed) graph <math|G<around*|(|V,E|)>> is the graph
  <math|T<around*|(|V,E<rprime|'>|)>> in which two vertices <math|v,w\<in\>V>
  are connected by an edge (arc) <math|e=v w\<in\>E<rprime|'>> if and only if
  there exists a (directed) path from <math|v> to <math|w> in <math|G>.

  <verbatim|transitive_closure> accepts one or two arguments, the input graph
  <math|G> and optionally the option <verbatim|weighted=true> (or simply
  <verbatim|weighted>) or the option <verbatim|weighted=false> (which is the
  default). The command returns the transitive closure <math|T> of the input
  graph <math|G>. If <math|G> is a digraph, then <math|T> is also a digraph.
  When <verbatim|weighted=true> is specified, the graph <math|T> is weighted
  such that the weight of edge <math|v\<space\>w\<in\>E<rprime|'>> is equal
  to the length (or weight, if <math|G> is weighted) of the shortest path
  from <math|v> to <math|w> in <math|G>.

  The lengths/weights of the shortest paths are obtained by the command
  <verbatim|allpairs_distance> if <math|G> is weighted resp.<nbsp>the command
  <verbatim|vertex_distance> if <math|G> is unweighted. Therefore <math|T> is
  constructed in at most <math|O<around*|(|<around*|\||V|\|><rsup|3>|)>> time
  if <verbatim|weighted=true> resp.<nbsp><math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>>
  time if <verbatim|weighted=false>.

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

  <center|<image|images/digraph3.eps|40%|||>>

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

  <center|<image|images/digraph4.eps|40%|||>>

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

  <center|<image|images/digraph5.eps|40%|||>>

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(transitive_closure(G,weighted=true))
    </input>
  </session>

  <center|<image|images/digraph6.eps|40%|||>>

  <subsection|Line graph>

  The command <verbatim|line_graph> is used for constructing the line graph
  of an undirected graph <math|G<around*|(|V,E|)>>.

  <verbatim|line_graph> accepts the input graph <math|G> as its only argument
  and returns the line graph <math|L<around*|(|G|)>> with
  <math|<around*|\||E|\|>> distinct vertices, one vertex for each edge in
  <math|E>. Two vertices <math|v<rsub|1>> and <math|v<rsub|2>> in
  <math|L<around*|(|G|)>> are adjacent if and only if the corresponding edges
  <math|e<rsub|1>,e<rsub|2>\<in\>E> have a common endpoint. The vertices in
  <math|L<around*|(|G|)>> are labeled with strings in form <verbatim|v-w>,
  where <math|e=v w\<in\>E>.

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

  <center|<image|images/linegraph.eps|40%|||>>

  <subsection|Plane dual graph>

  The command <verbatim|plane_dual> is used for constructing the plane dual
  graph of an undirected biconnected planar graph.

  <verbatim|plane_dual> accepts one argument, the input graph
  <math|G<around*|(|V,E|)>> or the list <math|F> of faces of a planar
  embedding of <math|G>, and returns the graph <math|H> with faces of
  <math|G> as its vertices in which two vertices are adjacent if and only if
  they share an edge as faces of <math|G>.

  Note that the concept of dual graph is usually defined for
  multigraphs<\footnote>
    See <hlink|https://en.wikipedia.org/wiki/Dual_graph|> for the strict
    definition of plane dual graph.
  </footnote>. By the strict definition, every planar multigraph has the
  corresponding dual multigraph; moreover, the dual of the latter is equal to
  the former. Since Giac does not support multigraphs, a simplified
  definition suitable for simple graphs is used.

  The algorithm runs in <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> time.

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

  <center|<image|images/cube.eps|40%|||>>

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

  <center|<image|images/dual.eps|40%|||>>

  A graph isomorphic to <math|D> is obtained when passing a list of faces of
  <math|H> to the <verbatim|plane_dual> command. The order of vertices is
  determined by the order of faces.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_planar(H,F)
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      plane_dual(F)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 12 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Random graphs>

  <subsection|Random general graphs>

  The commands <verbatim|random_graph> and <verbatim|random_digraph> are used
  for generating general graphs at random.

  <verbatim|random_graph> and <verbatim|random_digraph> both accept two
  arguments. The first argument is a positive integer <math|n> or a list of
  labels <math|L> of length <math|n>. The second argument is a positive real
  number <math|p\<less\>1> or a positive integer <math|m>. The command
  returns a random (di)graph on <math|n> vertices (using elements of <math|L>
  as vertex labels) in which each edge/arc is inserted with probability
  <math|p> or which contains exactly <math|m> edges/arcs chosen at random.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_digraph(10,0.2)
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 10 vertices and 15 arcs>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring,labels=false)
    </input>
  </session>

  <center|<image|images/rand7.eps|40%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(1000,0.01)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 1000 vertices and 4922
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
      is_biconnected(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(100,99)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 99 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_tree(G)
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(G,spring)
    </input>
  </session>

  <center|<image|images/rand8.eps|40%|||>>

  <subsection|Random bipartite graphs>

  The command <verbatim|random_bipartite_graph> is used for generating
  bipartite graphs at random.

  <verbatim|random_bipartite_graph> accepts two arguments. The first argument
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

  <center|<image|images/rand6.eps|40%|||>>

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

  The command <verbatim|random_tree> is used for generating tree graphs
  uniformly at random.

  <verbatim|random_tree> accepts a positive integer <math|n> as its only
  argument and returns a random tree generated on <math|n> nodes
  (i.e.<nbsp>inserts <math|n-1> edges in the initially empty graph).

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      T:=random_tree(30)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 29 edges>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(T,labels=false)
    </input>
  </session>

  <center|<image|images/rand5.eps|40%|||>>

  <subsection|Random planar graphs>

  The command <verbatim|random_planar_graph> is used for generating random
  planar graphs, controlling edge density and vertex connectivity.

  <verbatim|random_planar_graph> accepts two or three arguments, a positive
  integer <math|n> (or a list <math|L> of length <math|n>), a positive real
  number <math|p\<less\>1> and optionally an integer
  <math|k\<in\><around*|{|0,1,2,3|}>> (by default, <math|k=1>). The command
  returns a random <math|k>-connected planar graph on <math|n> vertices
  (using elements of <math|L> as vertex labels).

  The result is obtained by first generating a random maximal planar graph
  and then attempting to remove each edge with probability <math|p>,
  maintaining the <math|k>-connectivity of the graph (if <math|k=0>, the
  graph may be disconnected). The running time is
  <math|O<around*|(|<around*|\||V|\|>|)>> if <math|k=0>,
  <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> if
  <math|k\<in\><around*|{|1,2|}>> and <math|O<around*|(|<around*|\||V|\|><rsup|3>|)>>
  if <math|k=3>.

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

  <center|<image|images/rand1.eps|40%|||>>

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

  <center|<image|images/rand2.eps|40%|||>>

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

  <center|<image|images/rand3.eps|40%|||>>

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

  <center|<image|images/rand4.eps|40%|||>>

  <subsection|Random regular graphs>

  The command <verbatim|random_regular_graph> is used for generating random
  <math|d>-regular graphs on a certain set of vertices, using the algorithm
  presented in<nbsp><cite|steger><\footnote>
    See Algorithm<nbsp>2 on page<nbsp>2.
  </footnote>.

  <verbatim|random_regular_graph> accepts two mandatory arguments, a positive
  integer <math|n> (or a list <math|L> of length <math|n>) and a nonnegative
  integer <math|d>. Optionally, the option <verbatim|connected> may be
  specified as the third argument, indicating that the generated graph must
  be 1-connected (by default no restriction is made). The command creates
  <math|n> vertices (using elements of <math|L> as vertex labels) and returns
  a random <math|d>-regular (connected) graph on these vertices.

  Note that a <math|d>-regular graph on <math|n> vertices exists if and only
  if <math|n\<gtr\>d+1> and <math|n*d> is even. If these conditions are not
  met, <verbatim|random_regular_graph> returns an error.

  This algorithm generates regular graphs with approximately uniform
  probability. It means that for <math|n\<rightarrow\>\<infty\>> and <math|d>
  not growing so quickly with <math|n> the probability distribution converges
  to uniformity.

  The runtime of the algorithm is negligible for <math|n\<leqslant\>100>. For
  <math|n\<gtr\>200> the algorithm is considerably slower.

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

  <center|<image|images/regular.eps|40%|||>>

  <subsection|Random tournaments>

  The command <verbatim|random_tournament> is used for generating random
  tournaments. A <em|tournament> of order <math|n> is a graph obtained by
  assigning a direction to each edge in a complete graph <math|K<rsub|n>>
  (for each edge there are two possible directions).

  <verbatim|random_tournament> accepts a positive integer <math|n> (or a list
  <math|L> of length <math|n>) as its only argument and returns a random
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

  <center|<image|images/tnm.eps|40%|||>>

  <subsection|Random flow networks>

  <\session|giac|default>
    <\input|Giac] >
      \;
    </input>
  </session>

  <subsection|Randomizing edge weights>

  The command <verbatim|assign_edge_weights> is used for assigning weights to
  edges of a graph at random.

  <verbatim|assign_edge_weights> accepts two or three arguments. The first
  argument is always the input graph <math|G<around*|(|V,E|)>>. If only two
  arguments are given, the second one is an interval <verbatim|a..b> of real
  numbers. Otherwise, if three arguments are given, the second resp.<nbsp>the
  third argument is a positive integer <math|m> resp.<nbsp><math|n>. The
  command operates such that for each edge <math|e\<in\>E>, its weight is
  chosen uniformly from the real interval <math|<around*|[|a,b|)>> or from
  the set of integers lying between <math|m> and <math|n>, including both
  <math|m> and <math|n>. After assigning weights to all edges, the modified
  copy of <math|G> is returned.

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

  <center|<image|images/grid.eps|40%|||>>

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

  <center|<image|images/rand9.eps|40%|||>>

  <chapter|Modifying graphs>

  <section|Modifying vertices of a graph>

  <subsection|Adding and removing single vertices>

  For adding and removing vertices to/from graphs use the commands
  <kbd|add_vertex> and <kbd|delete_vertex>, respectively.

  The command <with|font-family|tt|add_vertex> accepts two arguments, a graph
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

    <\textput>
      Note that vertices already present in <math|G> won't be added. For
      example:
    </textput>

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

  The command <with|font-family|tt|delete_vertex> accepts two arguments, a
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

  <subsection|Adding and removing single edges>

  For adding and removing edges or arcs to/from graphs use the commands
  <kbd|add_edge> or <kbd|add_arc> and <kbd|delete_edge> and <kbd|delete_arc>,
  respectively.

  The command <with|font-family|tt|add_edge> accepts two arguments, an
  undirected graph <math|G<around|(|V,E|)>> and an edge or a list of edges or
  a trail of edges (entered as a list of vertices), and returns the copy of
  <math|G> with the specified edges inserted. Edge insertion implies creation
  of its endpoints if they are not already present.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      C4:=cycle_graph(4)
    <|unfolded-io>
      <\equation*>
        <text|C4: an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_edge(C4,[1,3])
    <|unfolded-io>
      <\equation*>
        <text|C4: an undirected unweighted graph with 4 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      add_edge(C4,[1,3,5,7])
    <|unfolded-io>
      <\equation*>
        <text|C4: an undirected unweighted graph with 6 vertices and 7 edges>
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

  <subsection|Accessing and modifying edge weights>

  The commands <kbd|get_edge_weight> and <with|font-family|tt|set_edge_weight>
  are used to access and modify the weight of an edge/arc in a weighted
  graph, respectively.

  <with|font-family|tt|set_edge_weight> accepts three arguments: a weighted
  graph <math|G<around|(|V,E|)>>, edge/arc <math|e\<in\>E> and the new weight
  <math|w>, which may be any number. It returns the modified copy of
  <math|G>.

  The command <with|font-family|tt|get_edge_weight> accepts two arguments, a
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

  The command <with|font-family|tt|contract_edge> is used for contracting
  (collapsing) edges in a graph.

  <with|font-family|tt|contract_edge> accepts two arguments, a graph
  <math|G<around|(|V,E|)>> and an edge/arc <math|e=<around|(|v,w|)>\<in\>E>,
  and merges <math|w> and <math|v> into a single vertex, deleting the edge
  <math|e>. The resulting vertex inherits the label of <math|v>. The command
  returns the modified graph <math|G<rprime|'>*<around|(|V\<setminus\><around|{|w|}>,E<rprime|'>|)>>.

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
  <with|font-family|tt|foldl> command. For example, if <math|G> is the
  complete graph <math|K<rsub|5>> and <math|k=2>,
  <math|e<rsub|1>=<around|{|1,2|}>> and <math|e<rsub|2>=<around|{|3,4|}>>,
  input:

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      foldl(contract_edge,K5,[1,2],[3,4])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Subdividing edges>

  The command <with|font-family|tt|subdivide_edges> is used for subdividing
  edges of a graph.

  <with|font-family|tt|subdivide_edges> accepts two or three arguments, a
  graph <math|G<around|(|V,E|)>>, a single edge/arc or a list of edges/arcs
  in <math|E> and optionally a positive integer <math|r> (which defaults to
  1). Each of the specified edges/arcs will be subdivided with exactly
  <math|r> new vertices, labeled with the smallest available integers. The
  resulting graph, which is homeomorphic to <math|G>, is returned.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=graph(%{[1,2],[2,3],[3,1]%})
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 3 vertices and 3 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      subdivide_edges(G,[2,3])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 4 vertices and 4 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      subdivide_edges(G,[[1,2],[1,3]])
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      subdivide_edges(G,[2,3],3)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 6 vertices and 6 edges>
      </equation*>
    </unfolded-io>
  </session>

  <section|Using attributes>

  <subsection|Graph attributes>

  The graph structure maintains a set of attributes as tag-value pairs which
  can be accessed and modified by the user.

  The command <with|font-family|tt|set_graph_attribute> is used for modifying
  the existing graph attributes or adding new ones. It accepts two arguments,
  a graph <math|G> and a sequence or list of graph attributes in form
  <with|font-family|tt|tag=value> where <with|font-family|tt|tag> is any
  string. Alternatively, attributes may be specified as a sequence of two
  lists <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attribute slots, which are meant to represent some
  global properties of the graph <math|G>, and returns the modified copy of
  <math|G>.

  The previously set graph attribute values can be fetched with the command
  <with|font-family|tt|get_graph_attribute> which accepts two arguments: a
  graph <math|G> and a sequence or list of tags. The corresponding values
  will be returned in a sequence or list, respectively. If some attribute is
  not set, <with|font-family|tt|undef> is returned as its value.

  To list all graph attributes of <math|G> for which the values are set, use
  the <with|font-family|tt|list_graph_attributes> command which takes
  <math|G> as its only argument.

  To discard a graph attribute, use the <with|font-family|tt|discard_graph_attribute>
  command. It accepts two arguments: a graph <math|G> and a sequence or list
  of tags to be cleared, and returns the modified copy of <math|G>.

  Two tags are normally being used by the <abbr|CAS> commands:
  <samp|directed> and <samp|weighted>, so it is not advisable to overwrite
  their values using this command (instead, use
  <with|font-family|tt|make_directed>, <with|font-family|tt|make_weighted>
  and <with|font-family|tt|underlying_graph> commands). Another attribute
  used internally is <samp|name>, which holds the name of the respective
  graph (as a string).

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

  <subsection|Vertex attributes>

  For every vertex of a graph, the list of attributes in form of tag-value
  pairs is maintained, which can be accessed and modified by the user.

  The command <with|font-family|tt|set_vertex_attribute> is used for
  modifying the existing vertex attributes or adding new ones. It accepts
  three arguments, a graph <math|G<around|(|V,E|)>>, a vertex <math|v\<in\>V>
  and a sequence or list of attributes in form
  <with|font-family|tt|tag=value> where <with|font-family|tt|tag> is any
  string. Alternatively, attributes may be specified as a sequence of two
  lists <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attributes of the vertex <math|v> and returns the
  modified copy of <math|G>.

  The previously set attribute values for <math|v> can be fetched with the
  command <with|font-family|tt|get_vertex_attribute> which accepts three
  arguments: <math|G>, <math|v> and a sequence or list of tags. The
  corresponding values will be returned in a sequence or list, respectively.
  If some attribute is not set, <with|font-family|tt|undef> is returned as
  its value.

  To list all attributes of <math|v> for which the values are set, use the
  <with|font-family|tt|list_vertex_attributes> command which takes two
  arguments, <math|G> and <math|v>.

  The <with|font-family|tt|discard_vertex_attribute> command is used for
  discarding attribute(s) assigned to some vertex <math|v\<in\>V>. It accepts
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

  <center|<image|images/rooted.eps|40%|||>>

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
  pairs is maintained, which can be accessed and modified by the user.

  The command <with|font-family|tt|set_edge_attribute> is used for modifying
  the existing edge attributes or adding new ones. It accepts three
  arguments, a graph <math|G<around|(|V,E|)>>, an edge/arc <math|e\<in\>E>
  and a sequence or list of attributes in form
  <with|font-family|tt|tag=value> where <with|font-family|tt|tag> is any
  string. Alternatively, attributes may be specified as a sequence of two
  lists <with|font-family|tt|[tag1,tag2,...]> and
  <with|font-family|tt|[value1,value2,...]>. The command sets the specified
  values to the indicated attributes of the edge/arc <math|e> and returns the
  modified copy of <math|G>.

  The previously set attribute values for <math|e> can be fetched with the
  command <with|font-family|tt|get_edge_attribute> which accepts three
  arguments: <math|G>, <math|e> and a sequence or list of tags. The
  corresponding values will be returned in a sequence or list, respectively.
  If some attribute is not set, <with|font-family|tt|undef> is returned as
  its value.

  To list all attributes of <math|e> for which the values are set, use the
  <with|font-family|tt|list_edge_attributes> command which takes two
  arguments, <math|G> and <math|e>.

  To discard attribute(s) assigned to <math|e> call the
  <with|font-family|tt|discard_edge_attribute> command, which accepts three
  arguments: <math|G>, <math|e> and a sequence or list of tags to be cleared,
  and returns the modified copy of <math|G>.

  The attributes <samp|weight>, <samp|color>, <samp|style>, <samp|pos> and
  <samp|temp> are also used internally. They hold the edge weight, color,
  line style, the coordinates of the weight label anchor (and also the
  coordinates of the arrow) and <verbatim|true> if the edge is temporary. If
  the color attribute is not set for an edge, the latter is drawn in blue,
  unless it is a temporary edge, in which case it is drawn in light grey. The
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

  <center|<image|images/dashed.eps|40%|||>>

  <chapter|Import and export>

  <section|Importing graphs>

  <subsection|Loading graphs from <with|font-family|ss|dot> files>

  The command <verbatim|import_graph> is used for importing a graph from text
  file in <samp|dot> format<\footnote>
    <hlink|https://en.wikipedia.org/wiki/DOT_(graph_description_language)|>
  </footnote>.

  <with|font-family|tt|import_graph> accepts a string
  <with|font-family|tt|filename> as its only argument and returns the graph
  constructed from instructions written in the file
  <with|font-family|tt|filename> or <with|font-family|tt|undef> on failure.
  The passed string should contain the path to a file in
  <with|font-family|ss|dot> format. The file extension
  <with|font-family|tt|.dot> may be omitted in the
  <with|font-family|tt|filename> since <with|font-family|ss|dot> is the only
  supported format. The alternative extension is <verbatim|.gv>,<\footnote>
    Although it is recommended to use <verbatim|.gv> as the extension for
    <samp|dot> files to avoid a certain confusion between different file
    types, Giac uses the <verbatim|.dot> extension because it coincides with
    the format name. This may change in the future.
  </footnote> which must be explicitly specified.

  If a relative path to the file is specified, i.e.<nbsp>if it does not
  contain a leading forward slash, the current working directory (which can
  be obtained by calling the <with|font-family|tt|pwd> command) will be used
  as the reference. The working directory can be changed by using the command
  <with|font-family|tt|cd>.

  For example, assume that the file <with|font-family|tt|example.dot> is
  saved in the directory <with|font-family|tt|Documents/dot/> with the
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
      G:=import_graph("Documents/dot/example.dot")
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

  <center|<image|images/ex.eps|40%|||>>

  <subsection|The <with|font-family|ss|dot> file format overview>

  Giac has some basic support for <with|font-family|ss|dot>
  language<footnote|For the complete syntax definition see
  <slink|https://www.graphviz.org/doc/info/lang.html>.>. Each <samp|dot> file
  is used to hold exactly one graph and should consist of a single instance
  of the following environment:

  <\verbatim-code>
    strict? (graph \| digraph) name? {

    \ \ \ \ ...

    }
  </verbatim-code>

  The keyword <with|font-family|tt|strict> may be omitted, as well as the
  <with|font-family|tt|name> of the graph, as indicated by the question
  marks. The former is used to differentiate between simple graphs (strict)
  and multigraphs (non-strict). Since Giac supports only simple graphs,
  <with|font-family|tt|strict> is redundant.

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

  Any line beginning with <with|font-family|tt|#> is ignored. C-like line and
  block comments are recognized and skipped as well.

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

  <subsection|Saving graphs in <with|font-family|ss|dot> format>

  <with|font-family|tt|export_graph> accepts two mandatory arguments, a graph
  <math|G> and a string <with|font-family|tt|filename>, and writes <math|G>
  to the file specified by <with|font-family|tt|filename>, which must be a
  path to the file, either relative or absolute; in the former case the
  current working directory will be used as the reference. If only two
  arguments are given the graph is saved in <with|font-family|ss|dot> format.
  The file name may be entered with or without <with|font-family|tt|.dot>
  extension. The command returns 1 on success and 0 on failure.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      export_graph(G,"Documents/dot/copy_of_philosophers")
    <|unfolded-io>
      <\equation*>
        1
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Saving graph drawings in <LaTeX> format>

  When calling the <kbd|export_graph> command, an optional third argument in
  form <with|font-family|tt|latex[=\<less\>params\<gtr\>]> may be given. In
  that case the drawing of <math|G> (obtained by calling the
  <with|font-family|tt|draw_graph> command) will be saved to the <LaTeX> file
  indicated by <with|font-family|tt|filename> (the extension
  <with|font-family|tt|.tex> may be omitted). Optionally, one can specify a
  parameter or list of parameters <with|font-family|tt|params> which will be
  passed to the <with|font-family|tt|draw_graph> command.

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
      export_graph(G,"Documents/st53.tex",latex=[spring,labels=false])
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

  <subsection|Listing vertices and edges>

  The command <verbatim|vertices> or <verbatim|graph_vertices>
  resp.<nbsp><verbatim|edges> is used for extracting set of vertices
  resp.<nbsp>set of edges from a graph.

  <verbatim|vertices> or <verbatim|graph_vertices> accepts a graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the set of
  vertices <math|V> in the same order in which they were created.

  <verbatim|edges> accepts one or two arguments, a graph
  <math|G<around*|(|V,E|)>> and optionally the identifier <verbatim|weights>.
  The command returns the set of edges <math|E> (in a non-meaningful order).
  If <verbatim|weights> is specified, each edge is paired with the
  corresponding weight (in this case <math|G> must be a weighted graph).

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
        <around|{|<around|[|0,1|]>,<around|[|0,3|]>,<around|[|1,3|]>,<around|[|2,1|]>,<around|[|3,0|]>,<around|[|3,2|]>|}>
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

  <subsection|Vertex degrees>

  The command <verbatim|vertex_degree> is used for computing the degree of a
  vertex, i.e.<nbsp>counting the vertices adjacent to it.

  <verbatim|vertex_degree> accepts two arguments, a graph
  <math|G<around*|(|V,E|)>> and a vertex <math|v\<in\>V>, and returns the
  cardinality of the set <math|<around*|{|w\<in\>V\<of\><around*|(|v,w|)>\<in\>E|}>>,
  i.e.<nbsp>the number of vertices in <math|V> which are adjacent to
  <math|v>.

  When dealing with directed graphs, one can also use the specialized command
  <verbatim|vertex_in_degree> resp.<nbsp><verbatim|vertex_out_degree> which
  accepts the same arguments as <verbatim|vertex_degree> but returns the
  number of arcs <math|<around*|(|w,v|)>\<in\>E> resp.<nbsp>the number of
  arcs <math|<around*|(|v,w|)>\<in\>E>, where <math|w\<in\>V>.

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

  <center|<image|images/graph1.eps|40%|||>>

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

  <center|<image|images/tournament.eps|40%|||>>

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

  To obtain the list of degrees of all vertices <math|v\<in\>V>, use the
  <verbatim|degree_sequence> command.

  <verbatim|degree_sequence> accepts a graph <math|G<around*|(|V,E|)>> as its
  only argument and returns the list of degrees of vertices from <math|V> in
  the same order as returned by the command <verbatim|vertices>. If <math|G>
  is a digraph, arc directions are ignored.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      degree_sequence(G)
    <|unfolded-io>
      <\equation*>
        <around|[|5,2,2,3,2,2,2,2,2|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Vertex adjacency>

  The command <verbatim|has_edge> is used for checking if two vertices in an
  undirected graph are adjacent or not.

  <verbatim|has_edge> accepts two arguments, the input graph
  <math|G<around*|(|V,E|)>> and a list <verbatim|[u,v]> where
  <math|u,v\<in\>V>. The command returns <verbatim|true> if <math|u v\<in\>E>
  and <verbatim|false> otherwise.

  For digraphs, there is the similar command <verbatim|has_arc> with the same
  input syntax. Note, however, that the order of vertices <math|u> and
  <math|v> matters this time.

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
      D:=digraph(trail(1,2,3,4,5,2))
    <|unfolded-io>
      <\equation*>
        <text|a directed unweighted graph with 5 vertices and 5 arcs>
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
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  The command <verbatim|neighbors> is used for obtaining the list of vertices
  in a graph that are adjacent to the particular vertex or the complete
  adjacency structure of the graph, in sparse form.

  <verbatim|neighbors> accepts one or two arguments. The first, mandatory
  argument is the input graph <math|G<around*|(|V,E|)>>. The second, optional
  argument is a vertex <math|v\<in\>V>. The command returns the list of
  neighbors of <math|v> in <math|G> if <math|v> is given and the list of
  lists of neighbors for all vertices in <math|V>, in order of
  <verbatim|vertices(G)>, otherwise.

  Note that <verbatim|neighbors> works for undirected as well as for directed
  graphs, but ignores edge directions in the latter case.

  <\session|giac|default>
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
  </session>

  The command <verbatim|departures> resp.<nbsp><verbatim|arrivals> is used
  for determining all neighbors of a vertex <math|v> in a digraph which are
  the heads resp.<nbsp>the tails of the corresponding arcs.

  <verbatim|departures> resp.<nbsp><verbatim|arrivals> accepts one or two
  arguments, the input digraph <math|G<around*|(|V,E|)>> and optionally a
  vertex <math|v\<in\>V>, and returns the list <math|L<rsub|v>> containing
  all vertices <math|w\<in\>V> for which <math|v w\<in\>E> resp.<nbsp><math|w
  v\<in\>E>. If <math|v> is omitted, the list of lists <math|L<rsub|v>> for
  every <math|v\<in\>V> is returned.

  <\session|giac|default>
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

  <center|<image|images/digraph7.eps|40%|||>>

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

  <subsection|Edge incidence>

  The command <verbatim|incident_edges> is used for obtaining edges incident
  to the given vertex in a graph.

  <verbatim|incident_edges> accepts two argument, the input graph
  <math|G<around*|(|V,E|)>> and a vertex <math|v\<in\>V> or a list of
  vertices <math|L\<subset\>V>. The command returns the list of edges
  <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|k>\<in\>E> which have <math|v>
  as one of its endpoints.

  Note that edge directions are ignored when <math|G> is a digraph. To obtain
  only outgoing or incoming edges, use the commands <verbatim|departures> and
  <verbatim|arrivals>, respectively.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=cycle_graph([1,2,3,4,5])
    <|unfolded-io>
      <\equation*>
        <text|C5: an undirected unweighted graph with 5 vertices and 5 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incident_edges(G,1)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|1,5|]>|}>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      incident_edges(G,[2,4,5])
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|1,5|]>,<around|[|2,3|]>,<around|[|3,4|]>,<around|[|4,5|]>|}>
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
        <around|{|<around|[|2,1|]>,<around|[|2,3|]>,<around|[|2,5|]>,<around|[|4,2|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <section|Algebraic properties>

  <subsection|Adjacency matrix>

  The command <verbatim|adjacency_matrix> is used for obtaining the adjacency
  matrix of a graph <math|G<around*|(|V,E|)>> where
  <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>.

  <verbatim|adjacency_matrix> accepts the input graph <math|G> as its only
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

  <center|<image|images/digraph8.eps|40%|||>>

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

  <subsection|Weight matrix>

  The command <verbatim|weight_matrix> is used for obtaining the weight
  matrix of a weighted graph <math|G<around*|(|V,E|)>> where
  <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>.

  <verbatim|weight_matrix> accepts the input graph <math|G> as its only
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

  <subsection|Incidence matrix>

  The command <verbatim|incidence_matrix> is used for obtaining the incidence
  matrix of a graph.

  <verbatim|incidence_matrix> accepts the input graph
  <math|G<around*|(|V,E|)>>, where <math|V=<around*|{|v<rsub|1>,v<rsub|2>,\<ldots\>,v<rsub|n>|}>>
  and <math|E=<around*|{|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|m>|}>>, as its
  only argument and returns the <math|n\<times\>m> matrix
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
        <around|{|<around|[|1,2|]>,<around|[|1,3|]>,<around|[|1,4|]>,<around|[|2,3|]>,<around|[|2,4|]>,<around|[|3,4|]>|}>
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

  <center|<image|images/digraph9.eps|40%|||>>

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      edges(DG)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|1,5|]>,<around|[|2,3|]>,<around|[|2,4|]>,<around|[|3,4|]>,<around|[|4,1|]>,<around|[|4,5|]>,<around|[|5,2|]>,<around|[|5,3|]>|}>
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

  <subsection|Characteristic polynomial>

  The command <verbatim|graph_charpoly> or <verbatim|charpoly> is used for
  obtaining the characteristic polynomial of an undirected graph.

  <verbatim|graph_charpoly> or <verbatim|charpoly> accepts one or two
  arguments, the input graph <math|G<around*|(|V,E|)>> and optionally a value
  or symbol <math|x>. The command returns <math|p<around*|(|x|)>>, where
  <math|p> is the characteristic polynomial of the adjacency matrix of
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

  <subsection|Graph spectrum>

  The command <verbatim|graph_spectrum> is used for obtaining the spectrum of
  eigenvalues of a graph.

  <verbatim|graph_spectrum> accepts the input graph <math|G> as its only
  argument and returns the list in which every element is an eigenvalue of
  the adjacency matrix of <math|G> paired with its multiplicity.

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

  The last result indicates that <verbatim|gs> and <verbatim|roots(p)> are
  equal.

  <subsection|Seidel spectrum>

  The command <verbatim|seidel_spectrum> is used for obtaining the Seidel
  spectrum of a graph.

  <verbatim|seidel_spectrum> accepts the input graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the list in
  which every element is an eigenvalue of the matrix <math|J-I-2*A> paired
  with its multiplicity. Here <math|J> is all-one <math|n\<times\>n> matrix,
  <math|I> is the identity matrix of order <math|n>, <math|A> is the
  adjacency matrix of <math|G> and <math|n=<around*|\||V|\|>>.

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
      seidel_spectrum(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|-3>|<cell|10>>|<row|<cell|5>|<cell|6>>>>>|)>
      </equation*>
    </unfolded-io>

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
      seidel_spectrum(P)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|-3>|<cell|5>>|<row|<cell|3>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Integer graphs>

  The command <verbatim|is_integer_graph> is used for determining if a graph
  is an integer graph, i.e.<nbsp>if its spectrum consists only of integers.

  <verbatim|is_integer_graph> accepts the input graph <math|G> as its only
  argument. The return value is <verbatim|true> if <math|G> is an integer
  graph and <verbatim|false> otherwise.

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
  </session>

  <section|Connectivity>

  <subsection|Vertex connectivity>

  The commands <verbatim|is_connected>, <verbatim|is_biconnected> and
  <verbatim|is_triconnected> are used for determining if a graph is
  connected, biconnected or triconnected, respectively.

  All three commands accept only one argument, the input graph
  <math|G<around*|(|V,E|)>>. They return <verbatim|true> if <math|G>
  possesses the required type of connectivity and <verbatim|false> otherwise.

  <math|G> is <em|connected> or <em|1-connected> if for every pair
  <math|v,w\<in\>V> there exists a path with endpoints <math|u> and <math|v>
  in <math|G> or in the underlying graph of <math|G> if the latter is
  directed.

  <math|G> is <em|biconnected> or <em|2-connected> if it remains connected
  after removing a vertex from <math|G>.

  <math|G> is <em|triconnected> or <em|3-connected> if it remains connected
  after removing a pair of distinct vertices from <math|G>.

  The strategy for checking 1- and 2-connectivity is to use depth-first
  search<nbsp>(see <cite|gibbons> and<nbsp><cite|tarjan72>). Both algorithms
  run in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time. The
  algorithm for checking 3-connectivity is quite simple but less efficient:
  it works by choosing a vertex <math|v\<in\>V> and checking if the subgraph
  induced by <math|V\<setminus\><around*|{|v|}>> is biconnected, moving on to
  the next vertex if so, and repeating the process until all vertices are
  visited exactly once or a non-biconnected subgraph is found for some
  <math|v>. In the latter case the input graph is not triconnected. The
  complexity of this algorithm is <math|O<around*|(|<around*|\||V|\|>*<around*|\||E|\|>|)>>.

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

  <subsection|Connected and biconnected components>

  The command <verbatim|connected_components>
  resp.<nbsp><verbatim|biconnected_components> is used for decomposing a
  graph into connected resp.<nbsp>biconnected components.

  <verbatim|connected_components> resp.<nbsp><verbatim|biconnected_components>
  accept the input graph <math|G<around*|(|V,E|)>> as its only argument and
  returns the minimal partition <math|<around*|{|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>|}>>
  of <math|V> such that the subgraph <math|G<rsub|i>\<subset\>G> induced by
  <math|V<rsub|i>> is connected resp.<nbsp>biconnected for each
  <math|i=1,2,\<ldots\>,k>. The partition is returned as a list of lists
  <math|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>>.

  The connected components of <math|G> are easily obtained by depth-first
  search in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.
  To find the biconnected components of <math|G>, <name|Tarjan>'s algorithm
  is used<nbsp><cite|tarjan72>, which also runs in linear time.\ 

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

  <center|<image|images/ccomp.eps|40%|||>>

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

  <center|<image|images/bic.eps|40%|||>>

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

  <subsection|Graph rank>

  The command <verbatim|graph_rank> is used for computing the rank of a
  graph.

  <verbatim|graph_rank> accepts one or two arguments, the input graph
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

  <subsection|Articulation points>

  The command <verbatim|articulation_points> is used for obtaining the
  articulation points of a graph, i.e.<nbsp>cut vertices, if any.

  <verbatim|articulation_points> accepts the input graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the list of
  articulation points of <math|G>. A vertex <math|v\<in\>V> is an
  <em|articulation point> of <math|G> if the subgraph <math|H\<subset\>G>
  induced by <math|V\<setminus\><around*|{|v|}>> is disconnected.

  The articulation points of <math|G> are found by depth-first serach in
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
      articulation_points(cycle_graph(1,2,3,4))
    <|unfolded-io>
      <\equation*>
        <around|[||]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Strongly connected components>

  The command <verbatim|strongly_connected_components> is used for
  decomposing a graph into strongly connected components. A (di)graph
  <math|H> is <em|strongly connected> if for each pair
  <math|<around*|(|v,w|)>> of distinct vertices in <math|H> there is a
  (directed) path from <math|v> to <math|w> in <math|H>.

  <verbatim|strongly_connected_components> accepts the input graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the minimal
  partition <math|<around*|{|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>|}>> of
  <math|V> such that the subgraph <math|G<rsub|i>\<subset\>G> induced by
  <math|V<rsub|i>> is strongly connected for each <math|i=1,2,\<ldots\>,k>.
  The result is returned as a list of lists
  <math|V<rsub|1>,V<rsub|2>,\<ldots\>,V<rsub|k>>.

  The strategy is to use <name|Tarjan>'s algorithm for strongly connected
  components<nbsp><cite|tarjan72>, which runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  Note that an undirected graph is strongly connected if and only if it is
  connected.

  The command <verbatim|is_strongly_connected> can be used to determine if
  the given graph <math|G> is strongly connected. It accepts <math|G> as its
  only argument and returns <verbatim|true> if <math|G> has exactly one
  strongly connected component and <verbatim|false> otherwise.

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

  <center|<image|images/digraph10.eps|40%|||>>

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
  </session>

  <subsection|Edge connectivity>

  <section|Trees>

  <subsection|Tree graphs>

  The command <verbatim|is_tree> is used for determining if the particular
  graph is a tree. An undirected graph <math|G<around*|(|V,E|)>> is a
  <em|tree> if <math|<around*|\||V|\|>=<around*|\||E|\|>+1> and <math|G> is
  connected.

  <verbatim|is_tree> accepts the input graph <math|G> as its only argument
  and returns <verbatim|true> if <math|G> is a tree and <verbatim|false>
  otherwise.

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

  The command <verbatim|is_forest> is used for determining if the particular
  graph is a <em|forest>, i.e.<nbsp>if its connected components are all
  trees.

  <verbatim|is_forest> accepts the input graph <math|G> as its only argument
  and returns <verbatim|true> if <math|G> is a forest and <verbatim|false>
  otherwise.

  The only expensive step in the algorithm is the decomposition of <math|G>
  to connected components. Therefore the algorithm runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      L:=[]:; for k from 10 to 30 do L.append(random_tree(k)); od:;
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=disjoint_union(op(L))
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 420 vertices and 399 edges>
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
      is_forest(G)
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Height of a tree>

  The command <verbatim|tree_height> is used for determining the height of a
  tree with respect to the specified root node. The <em|height> of a tree is
  the length of the longest path in that tree that has the root node as one
  of its endpoints.

  <verbatim|tree_height> accepts two arguments, the input tree graph
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

  The command <verbatim|lowest_common_ancestor> determines the <em|lowest
  common ancestor> (<abbr|LCA>) of a pair of nodes in a tree, or for every
  element of a list of such pairs.

  <verbatim|lowest_common_ancestor> accepts two mandatory arguments, the
  input tree graph <math|G<around*|(|V,E|)>> and the root node
  <math|r\<in\>V>. There are two possibilities for specifying the nodes to
  operate on: either the nodes <math|u,v\<in\>V> are given as the third and
  the fourth argument, or a list of pairs of nodes in form
  <verbatim|[[u1,v1],[u2,v2],...,[uk,vk]]>, where
  <math|u<rsub|i>,v<rsub|i>\<in\>V> and <math|u<rsub|i>\<neq\>v<rsub|i>> for
  <math|i=1,2,\<ldots\>,k>, is given as the third argument. The command
  returns the <abbr|LCA> of <math|u> and <math|v> or the list containing
  <abbr|LCA> of every pair of nodes <math|u<rsub|i>,v<rsub|i>> for
  <math|i=1,2,\<ldots\>,k>.

  The strategy is to use <name|Tarjan>'s offline <abbr|LCA>
  algorithm<nbsp><cite|lca>. The implementation is simple and uses the
  disjoint-set (union-find) data structure. It runs in nearly linear time.

  In the following example, the algorithm efficiency is tested on a large
  random tree with 10000 nodes. The lowest common ancestors for the list
  <math|L> cotaining 100 pairs of vertices, chosen at random, need to be
  determined.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_tree(10000)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10000 vertices and 9999
        edges>
      </equation*>

      <timing|3.562 sec>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      V:=vertices(G):; L:=[]:; for k from 1 to 100 do L.append(rand(2,V));
      od:;
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<math-up|Done>,<math-up|Done>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      lowest_common_ancestor(G,0,L):;
    <|unfolded-io>
      <timing|9.409 sec>
    </unfolded-io>
  </session>

  <section|Distance>

  <subsection|Vertex distance>

  <subsection|All-pairs vertex distance>

  <subsection|Diameter of a graph>

  <subsection|Girth of a graph>

  The commands <verbatim|girth> and <verbatim|odd_girth> are used for
  computing the (odd) girth of an undirected unweighted graph.

  <verbatim|girth> resp.<nbsp><verbatim|odd_girth> accepts the input graph
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
      G:=graph("petersen")
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 10 vertices and 15 edges>
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
      G:=complete_binary_tree(2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 7 vertices and 6 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      girth(G)
    <|unfolded-io>
      <\equation*>
        +\<infty\>
      </equation*>
    </unfolded-io>
  </session>

  <section|Acyclic graphs>

  <subsection|Checking if a graph is acyclic>

  The command <verbatim|is_acyclic> is used for checking that the given
  digraph has no directed cycle. A directed graph with no directed cycle is
  said to be <em|acyclic>.

  <verbatim|is_acyclic> accepts the input digraph <math|G<around*|(|V,E|)>>
  as its only argument and returns <verbatim|true> if <math|G> is acyclic and
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

  The command <verbatim|topologic_sort> or <verbatim|topological_sort> is
  used for finding a linear ordering of vertices of an acyclic digraph which
  is consistent with the arcs of the digraph.

  <verbatim|topologic_sort> accepts the input acyclic digraph
  <math|G<around*|(|V,E|)>> as its only argument and returns the list of
  vertices of <math|G> in a particular order: a vertex <math|u> precedes a
  vertex <math|v> if <math|u*v\<in\>E>, i.e.<nbsp>if there is an arc from
  <math|u> to <math|v>.

  The strategy is to apply <name|Kahn>'s algorithm<nbsp><cite|kahn>, which
  runs in <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

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

  <subsection|<samp|st> ordering>

  The command <verbatim|st_ordering> is used for finding a
  <samp|st>-orientation in an undirected biconnected graph with respect to
  the given source and sink nodes.

  <verbatim|st_ordering> accepts three or four arguments: the input graph
  <math|G<around*|(|V,E|)>>, a vertex <math|s\<in\>V> called the <em|source>,
  a vertex <math|t\<in\>V> called the <em|target> or <em|sink> such that
  <math|s t\<in\>E> and optionally an unassigned identifier <math|D>. The
  command returns the permutation <math|\<sigma\>> which defines a particular
  order of vertices in <math|V>. That ordering defines the orientation for
  each edge <math|e\<in\>E>, which causes <math|G> to become acyclic with a
  single source <math|s> and sink <math|t>. The ordering defined by
  <math|\<sigma\>> is the topological ordering of the resulting digraph. If
  the optional argument <math|D> is given, the digraph is stored to it.

  The orientation of <math|e=u v\<in\>E> is determined by the numbers
  <math|n> and <math|m> of its endpoints <math|u> and <math|v>, respectively,
  which are assigned by the permutation <math|\<sigma\>>. If
  <math|n\<less\>m>, then <math|u> is the head and <math|v> is the tail of
  the corresponding arc, and vice versa otherwise.

  An undirected graph <math|G> has a <samp|st>-orientation if and only if
  <math|G> is biconnected. Hence, if this condition is not met by the input
  graph, <verbatim|st_ordering> returns an error. Otherwise,
  <samp|st>-ordering is found by applying the algorithm of <name|Even> and
  <name|Tarjan><nbsp><cite|even>, which runs in
  <math|O<around*|(|<around*|\||V|\|>+<around*|\||E|\|>|)>> time.

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
      sigma:=st_ordering(G,a,d,D)
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

  <center|<image|images/digraph11.eps|40%|||>>

  <section|Vertex matching>

  <subsection|Maximum matching>

  The command <verbatim|maximum_matching> is used for finding maximum
  matching in undirected graphs.

  <verbatim|maximum_matching> accepts the input graph
  <math|G<around*|(|V,E|)>> as its only argument and returns a list of edges
  <math|e<rsub|1>,e<rsub|2>,\<ldots\>,e<rsub|m>\<in\>E> such that
  <math|e<rsub|i>> and <math|e<rsub|j>> are not adjacent (i.e.<nbsp>have no
  common endpoints) for all <math|1\<leqslant\>i\<less\>j\<leqslant\>m>,
  under condition that <math|m> is maximal. Edges <math|e<rsub|k>> for
  <math|k=1,\<ldots\>,m> represent the matched pairs of vertices in <math|G>.

  The command applies <name|Edmonds>' blossom algorithm<\footnote>
    For a good description of the blossom algorithm, see
    <hlink|https://en.wikipedia.org/wiki/Blossom_algorithm|>.
  </footnote><nbsp><cite|edmonds>, which finds maximum matching in
  <math|O<around*|(|<around*|\||V|\|><rsup|2>*<around*|\||E|\|>|)>> time.

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
      maximum_matching(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,3|]>,<around|[|6,4|]>,<around|[|5,2|]>|}>
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

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,maximum_matching(G)),labels=false)
    </input>
  </session>

  <center|<image|images/matching1.eps|40%|||>>

  <subsection|Maximum matching in bipartite graphs>

  If the input graph is bipartite, maximum matching can be found much faster
  by using the <verbatim|bipartite_matching> command which applies the
  algorithm of <name|Hopcroft> and <name|Karp><nbsp><cite|hopcroft>.

  <verbatim|bipartite_matching> accepts an undirected, unweighted bipartite
  graph <math|G> as its only argument and returns a sequence containing two
  elements: the size of the maximum matching and the list of edges connecting
  matched pairs of vertices. The algorithm runs in
  <math|O<around*|(|<around*|\||E|\|>*<sqrt|<around*|\||V|\|>>|)>> time.

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
      n,M:=bipartite_matching(G)
    <|unfolded-io>
      <\equation*>
        10,<around|{|<around|[|0,1|]>,<around|[|2,3|]>,<around|[|4,5|]>,<around|[|6,7|]>,<around|[|8,9|]>,<around|[|10,13|]>,<around|[|11,18|]>,<around|[|12,15|]>,<around|[|14,17|]>,<around|[|16,19|]>|}>
      </equation*>
    </unfolded-io>

    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_edges(G,M))
    </input>
  </session>

  <center|<image|images/matching2.eps|40%|||>>

  <section|Cliques>

  <subsection|Clique graphs>

  The graph is a <em|clique> if it is complete, i.e.<nbsp>if each two of its
  vertices are adjacent to each other. To check if a graph is a clique one
  can use the <kbd|is_clique> command.

  <kbd|is_clique> accepts a graph <math|G<around*|(|V,E|)>> as its only
  argument and returns <kbd|true> if <math|G> is a complete graph; else the
  returned value is <kbd|false>.

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

  <subsection|Triangle-free graphs>

  The command <verbatim|is_triangle_free> is used for determining is the
  particular graph triangle-free. A graph is <em|triangle-free> if it
  contains no clique of cardinality equal to 3, and hence no cliques with
  cardinality greater than two.

  <verbatim|triangle_free> accepts the input graph <math|G> as its only
  argument and returns <verbatim|true> if <math|G> is triangle-free and
  <verbatim|false> otherwise.

  The strategy is to check whether <math|tr <around*|(|A<rsup|3>|)>=0>, where
  <math|A> is the adjacency matrix of <math|G>. If the last equality holds,
  the graph is triangle-free. This method is very fast as just one matrix
  multiplication needs to be carried out completely. Additionally, the matrix
  <math|A> is sparse, so a large number of vertices usually does not cause
  memory problems.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_triangle_free(graph("soccerball"))
    <|unfolded-io>
      <\equation*>
        <text|true>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      is_triangle_free(graph("tetrahedron"))
    <|unfolded-io>
      <\equation*>
        <text|false>
      </equation*>
    </unfolded-io>
  </session>

  \;

  <subsection|Maximal cliques>

  Each subgraph of a graph <math|G<around*|(|V,E|)>> which is itself a
  complete graph is called a clique in <math|G>. A clique is <em|maximal> if
  it cannot be extended by adding more vertices from <math|V> to it. To count
  all maximal cliques in a graph one can use the <kbd|clique_stats> command.

  <kbd|clique_stats> accepts <math|G> as the only mandatory argument. If it
  is the only argument given, the command returns a list of pairs, each pair
  consisting of two integers: clique cardinality <math|k> (first) and the
  number <math|n<rsub|k>\<gtr\>0> of <math|k>-cliques in <math|G> (second).
  Therefore, the sum of second members of all returned pairs is equal to the
  total count of all maximal cliques in <math|G>. As an optional second
  argument one may give a positive integer <math|k> or an interval
  <math|m><nbsp>..<nbsp><math|n> with integer bounds. In the first case only
  the number of <math|k>-cliques for the given <math|k> is returned; in the
  second case, only cliques with cardinality between <math|m> and <math|n>
  (inclusive) are counted.

  The strategy used to find all maximal cliques is a variant of the algorithm
  of <name|Bron> and <name|Kerbosch> as described in<nbsp><cite|tomita>. Its
  worst-case running time is <math|O<around*|(|3<rsup|<around*|\||V|\|>/3>|)>>.
  However, the performance is usually almost instantaneous for graphs with
  100 vertices or less.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(50,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 50 vertices and 588 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|3>|<cell|14>>|<row|<cell|4>|<cell|185>>|<row|<cell|5>|<cell|370>>|<row|<cell|6>|<cell|201>>|<row|<cell|7>|<cell|47>>|<row|<cell|8>|<cell|5>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(100,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 100 vertices and 2461
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G,5)
    <|unfolded-io>
      <\equation*>
        3124
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(500,0.25)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 500 vertices and 31257
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_stats(G,5..7)
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|5>|<cell|153444>>|<row|<cell|6>|<cell|18486>>|<row|<cell|7>|<cell|355>>>>>|)>
      </equation*>

      <timing|1.218 sec>
    </unfolded-io>
  </session>

  <subsection|Maximum clique>

  The largest maximal clique in the graph <math|G<around*|(|V,E|)>> is called
  <em|maximum clique>. The command <kbd|maximum_clique> can be used to find
  one in the given graph.

  <kbd|maximum_clique> accepts the graph <math|G> as its only argument and
  returns maximum clique in <math|G> as a list of vertices. The clique may
  subsequently be extracted from <math|G> using the command
  <kbd|induced_subgraph>.

  The strategy used to find maximum clique is an improved variant of the
  classical algorithm by <name|Carraghan> and <name|Pardalos> developed by
  <name|Östergård> in<nbsp><cite|ostergard>.

  In the following examples, the maximum cliques were obtained almost
  instantly.

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
        <text|an undirected unweighted graph with 300 vertices and 13380
        edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      maximum_clique(G)
    <|unfolded-io>
      <\equation*>
        <around|[|46,64,144,183,208,241,244,261|]>
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

  <center|<image|images/cliq.eps|40%|||>>

  <subsection|Minimum clique cover>

  The <em|minimum clique cover> for the graph <math|G<around*|(|V,E|)>> is
  the smallest set <math|S=<around*|{|C<rsub|1>,C<rsub|2>,\<ldots\>,C<rsub|k>|}>>
  of cliques in <math|G> such that for every <math|v\<in\>V> there exists
  <math|i\<leqslant\>k> such that <math|v\<in\>C<rsub|i>>. In Giac, such
  cover can be obtained by calling the <kbd|clique_cover> command.

  <kbd|clique_cover> accepts graph <math|G> as its mandatory argument and
  returns the smallest possible cover. Optionally, a positive integer may be
  passed as the second argument. In that case the requirement that <math|k>
  is less or equal to the given integer is set. If no such cover is found,
  <kbd|clique_cover> returns empty list.

  The strategy is to find the minimal vertex coloring in the complement
  <math|G<rsup|c>> of <math|G> (note that these two graphs share the same set
  of vertices). Each set of equally colored vertices in <math|G<rsup|c>>
  corresponds to a clique in <math|G>. Therefore, the color classes of
  <math|G<rsup|c>> map to the elements <math|C<rsub|1>,\<ldots\>,C<rsub|k>>
  of the minimal clique cover in <math|G>.

  There is a special case in which <math|G> is triangle-free, which is
  treated separately. Such a graph <math|G> contains only 1- and 2-cliques;
  in fact, every clique cover in <math|G> consists of a matching <math|M>
  together with the singleton cliques (i.e.<nbsp>the isolated vertices which
  remain unmatched). The total number of cliques in the cover is equal to
  <math|<around*|\||V|\|>-<around*|\||M|\|>>, hence to find the minimal cover
  one just needs to find maximum matching in <math|G>, which can be done in
  polynomial time.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(30,0.2)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 30 vertices and 88 edges>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(G)
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|0,2,7|]>,<around|[|1,3|]>,<around|[|4,10,13,14|]>,<around|[|5,6|]>,<around|[|8,15,26|]>,<around|[|9,22,25|]>,<around|[|11,16|]>,<around|[|12,17,23|]>,<around|[|18,20,24|]>,<around|[|19,28|]>,<around|[|21,27,29|]>|}>
      </equation*>
    </unfolded-io>

    <\textput>
      To find minimal clique cover in the truncated icosahedral graph it
      suffices to find maximum matching, since it is triangle-free.
    </textput>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      clique_cover(graph("soccerball"))
    <|unfolded-io>
      <\equation*>
        <around|{|<around|[|1,2|]>,<around|[|3,16|]>,<around|[|4,5|]>,<around|[|6,7|]>,<around|[|8,49|]>,<around|[|9,10|]>,<around|[|11,12|]>,<around|[|13|]>,<around|[|14,15|]>,<around|[|17,18|]>,<around|[|19,20|]>,<around|[|21,22|]>,<around|[|23|]>,<around|[|24,25|]>,<around|[|26,30|]>,<around|[|27,28|]>,<around|[|29,48|]>,<around|[|31,32|]>,<around|[|33,46|]>,<around|[|34,35|]>,<around|[|36,37|]>,<around|[|38,39|]>,<around|[|40|]>,<around|[|41,42|]>,<around|[|43,44|]>,<around|[|45,47|]>,<around|[|50|]>,<around|[|51,52|]>,<around|[|53,54|]>,<around|[|55,57|]>,<around|[|56,60|]>,<around|[|58,59|]>|}>
      </equation*>
    </unfolded-io>

    <\textput>
      The vertices of Petersen graph can be covered with five, but not with
      three cliques.
    </textput>

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
        <around|{|<around|[|0,1|]>,<around|[|2,3|]>,<around|[|4,9|]>,<around|[|5,7|]>,<around|[|6,8|]>|}>
      </equation*>
    </unfolded-io>
  </session>

  <section|Vertex coloring>

  To <em|color> vertices of a graph <math|G<around*|(|V,E|)>> means to assign
  to each vertex <math|v\<in\>V> a positive integer. Each integer represents
  a distinct color. The key property of a graph coloring is that the colors
  of adjacent vertices must differ from one another. Two different colorings
  of <math|G> may use different number of colors.

  <subsection|Greedy coloring>

  The command <kbd|greedy_color> is used for coloring vertices of a graph in
  a greedy fashion.

  <kbd|greedy_color> accepts one mandatory argument, the input graph
  <math|G>. Optionally, a permutation <math|p> of order
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

  <center|<image|images/color1.eps|40%|||>>

  The first six positive integers are always mapped to the standard Xcas
  colors, as indicated in Table<nbsp><reference|tab:colors>.<\float|float|tbh>
    <big-table|<center|<tabular|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|1|1|-1|cell-tborder|0ln>|<cwith|1|1|1|-1|cell-bborder|1ln>|<cwith|2|2|1|-1|cell-tborder|1ln>|<cwith|1|1|1|1|cell-lborder|0ln>|<cwith|1|1|2|2|cell-rborder|0ln>|<table|<row|<cell|<em|value>>|<cell|<em|color>>>|<row|<cell|1>|<cell|red>>|<row|<cell|2>|<cell|green>>|<row|<cell|3>|<cell|yellow>>|<row|<cell|4>|<cell|blue>>|<row|<cell|5>|<cell|magenta>>|<row|<cell|6>|<cell|cyan>>|<row|<cell|7>|<cell|black>>>>>>|<label|tab:colors>interpretation
    of abstract vertex/edge colors in Xcas>
  </float> Note that the color 0 (black) and color 7 (white) are swapped; a
  vertex with color 0 is <em|uncolored> or <em|white>, and vertex with color
  7 is black. Also note that Xcas will map the numbers greater than 7 to
  colors too, but the number of available colors is limited.

  <subsection|Minimal coloring>

  The vertex coloring of <math|G> is <em|minimal> (or <em|optimal>) if the
  smallest possible number of colors is used. To obtain such a coloring use
  the command <kbd|minimal_vertex_coloring>.

  <kbd|minimal_vertex_coloring> accepts one mandatory argument, the graph
  <math|G>. Optionally, a symbol <kbd|sto> may be passed as the second
  argument. The command returns the vertex colors in order of
  <kbd|vertices(G)> or, if the second argument is given, stores the colors as
  vertex attributes and returns the modified copy of <math|G>.

  Giac requires the <abbr|GLPK> library<\footnote>
    <abbr|GNU> Linear Programming Kit, <hlink|https://www.gnu.org/software/glpk/|>
  </footnote> to solve the minimal vertex coloring problem (<abbr|MVCP>),
  which is converted to the equivalent integer linear programming problem and
  solved by using the branch-and-bound method with specific branch/backtrack
  techniques <cite|diaz>. Lower and upper bounds for the number of colors
  <math|n> are obtained by finding a maximal clique (<math|n> cannot be lower
  than its cardinality) and by using the heuristic proposed by <name|Brélaz>
  in<nbsp><cite|brelaz> (which will use at least <math|n> colors),
  respectively.

  The algorithm is usually fast for graphs up to 40 vertices and for sparse
  graphs in general. For larger, denser graphs (e.g.<nbsp>with edge density
  around 0.5, which are the most difficult ones) one may have to wait for
  several minutes, even hours, and sometimes for a practically infinite time.
  Note that <abbr|MVCP> is a <abbr|NP>-hard problem, which means that no
  polynomial (i.e.<nbsp>efficient) algorithm is known.

  In the following example, the Grotzsch graph is colored with minimal number
  of colors by first finding the coloring and then assigning it to the graph
  by using the <verbatim|highlight_vertex> command.

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

  <center|<image|images/grotzsch.eps|40%|||>>

  To illustrate the combinatorial explosion which characterizes <abbr|MVCP>
  one can use the following example. Note that finding an optimal coloring in
  <math|G> is equivalent to finding the minimal clique cover in its
  complement <math|G<rsup|c>>.

  First, a random graph <math|G> with 40 vertices and edge density 0.5 is
  generated. Such a graph is considered to be relatively small.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      G:=random_graph(40,0.5)
    <|unfolded-io>
      <\equation*>
        <text|an undirected unweighted graph with 40 vertices and 393 edges>
      </equation*>
    </unfolded-io>
  </session>

  The chromatic number is computed by solving <abbr|MVCP> via
  <verbatim|minimal_vertex_coloring> in less than a second.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      chromatic_number(G)
    <|unfolded-io>
      <\equation*>
        8
      </equation*>

      <timing|273 msec>
    </unfolded-io>
  </session>

  Next, all maximal cliques in <math|G<rsup|c>> are counted using the
  <verbatim|clique_stats> command.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      S:=clique_stats(graph_complement(G))
    <|unfolded-io>
      <\equation*>
        <around*|(|<tabular*|<tformat|<cwith|1|-1|1|1|cell-halign|c>|<cwith|1|-1|1|1|cell-lborder|0ln>|<cwith|1|-1|2|2|cell-halign|c>|<cwith|1|-1|2|2|cell-rborder|0ln>|<table|<row|<cell|3>|<cell|5>>|<row|<cell|4>|<cell|66>>|<row|<cell|5>|<cell|238>>|<row|<cell|6>|<cell|89>>|<row|<cell|7>|<cell|3>>>>>|)>
      </equation*>
    </unfolded-io>

    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      n:=sum(col(S,1))
    <|unfolded-io>
      <\equation*>
        401
      </equation*>
    </unfolded-io>
  </session>

  To obtain the minimal coloring, a naive algorithm must check if a
  combination of <math|2,3,\<ldots\>,8> maximal cliques forms a cover in
  <math|G<rsup|c>>. In the worst case it will check
  <math|<big|sum><rsub|k=2><rsup|8><binom|n|k>> combinations, the number
  which is computed by the next command line.

  <\session|giac|default>
    <\unfolded-io>
      \<gtr\>\ 
    <|unfolded-io>
      sum(comb(n,k),k=2..8)
    <|unfolded-io>
      <\equation*>
        15776262844602110
      </equation*>
    </unfolded-io>
  </session>

  The magnitude of the above result (<math|\<approx\>1.6\<times\>10<rsup|17>>)
  indicates that the algorithm would run practically forever. (In particular,
  assuming that the algorithm requires <math|1
  \<mu\><with|math-font-family|rm|s>> to test one combination of cliques, the
  runtime is about 5070 years.) The fact that <verbatim|chromatic_number>
  required only quarter of a second to obtain minimal coloring indicates that
  the implemented algorithm is far more sophisticated than a simple
  brute-force approach.

  Note that solving <abbr|MVCP> for different graphs of exactly the same size
  (but which do not share the same edge structure) may take quite different
  time in each instance. For example, the time required to color a graph with
  50 vertices and edge density 0.5 may take any value between six seconds and
  six minutes. Also note that some graphs will take exponential time (that
  is, \Pforever\Q) to obtain the coloring. Table<nbsp><reference|tab:times>
  shows the runtimes (in seconds) of solving <abbr|MVCP> for random graphs
  with <math|10*k> vertices and edge density <math|<frac|d|10>> for
  <math|k=1,2,\<ldots\>,7> and <math|d=1,2,\<ldots\>,5>, using Intel i3-7130U
  processor at 2.70<nbsp>GHz. For each pair <math|<around*|(|k,d|)>>, ten
  graphs were generated and the average runtime was recorded in the
  table.<\float|float|tbh>
    <big-table|<tabular|<tformat|<cwith|1|1|2|2|cell-row-span|1>|<cwith|1|1|2|2|cell-col-span|5>|<cwith|1|1|1|1|cell-row-span|2>|<cwith|1|1|1|1|cell-col-span|1>|<cwith|1|2|1|2|cell-valign|c>|<cwith|1|2|1|2|cell-halign|c>|<cwith|3|7|1|1|cell-halign|c>|<cwith|2|2|3|6|cell-halign|c>|<cwith|3|7|2|-1|cell-halign|L.>|<twith|table-width|1par>|<twith|table-hmode|exact>|<cwith|1|-1|1|-1|cell-tborder|0ln>|<cwith|1|-1|1|-1|cell-bborder|0ln>|<cwith|1|-1|1|-1|cell-lborder|0ln>|<cwith|1|-1|1|-1|cell-rborder|0ln>|<cwith|3|3|1|-1|cell-tborder|1ln>|<cwith|1|2|1|-1|cell-bborder|1ln>|<cwith|3|3|1|-1|cell-bborder|0ln>|<cwith|4|4|1|-1|cell-tborder|0ln>|<cwith|3|3|1|1|cell-lborder|0ln>|<cwith|3|3|6|6|cell-rborder|0ln>|<cwith|7|7|1|-1|cell-tborder|0ln>|<cwith|6|6|1|-1|cell-bborder|0ln>|<cwith|7|7|1|-1|cell-bborder|1ln>|<cwith|7|7|1|1|cell-lborder|0ln>|<cwith|7|7|6|6|cell-rborder|0ln>|<table|<row|<cell|<strong|number
    of vertices>>|<cell|<strong|edge density>>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|<strong|0.1>>|<cell|<strong|0.2>>|<cell|<strong|0.3>>|<cell|<strong|0.4>>|<cell|<strong|0.5>>>|<row|<cell|<strong|10>>|<cell|0.0008892>|<cell|0.0010818>|<cell|0.0006116>|<cell|0.0005825>|<cell|0.0006331>>|<row|<cell|<strong|20>>|<cell|0.0014682>|<cell|0.00378>|<cell|0.0038329>|<cell|0.0077918>|<cell|0.0067932>>|<row|<cell|<strong|30>>|<cell|0.0021163>|<cell|0.0173315>|<cell|0.0372468>|<cell|0.111016>|<cell|0.143657>>|<row|<cell|<strong|40>>|<cell|0.0252134>|<cell|0.177707>|<cell|0.626175>|<cell|1.67746>|<cell|2.31473>>|<row|<cell|<strong|50>>|<cell|0.130089>|<cell|1.83819>|<cell|17.3564>|<cell|120.181>|<cell|246.161>>>>>|<label|tab:times>average
    runtime of the algorithm for minimal coloring (in seconds)>
  </float>

  <subsection|Chromatic number>

  The command <verbatim|chromatic_number> is used for exact computation and
  approximation of the chromatic number of a graph.

  <verbatim|chromatic_number> accepts one mandatory argument, the input graph
  <math|G<around*|(|V,E|)>>, and optionally a second argument. To obtain only
  upper and lower bound for the chromatic number (which is much faster than
  computing exactly) the option <verbatim|approx> or <verbatim|interval>
  should be passed as the second argument. Alternatively, an unassigned
  identifier is passed as the second argument; in that case the corresponding
  coloring will be stored to it in form of a list of colors of the individual
  vertices, ordered as in <verbatim|vertices(G)>.

  The command returns the chromatic number <math|\<chi\><rsub|G>> of the
  graph <math|G> in the case of exact computation. If the option
  <verbatim|approx> or <verbatim|interval> is given, an interval
  <verbatim|lb..ub> is returned, where <verbatim|lb> is the best lower bound
  and <verbatim|ub> the best upper bound for <math|\<chi\><rsub|G>> found by
  the algorithm.

  The strategy is call <verbatim|minimal_vertex_coloring> in the case of
  exact computation. When approximating the chromatic number, the algorithm
  will establish the lower bound by searching for maximum clique. The timeout
  for this operation is set to 5 seconds as it can be time consuming. If the
  maximum clique is not found after that time, the largest clique found is
  used. Then, an upper bound is established by by using the heuristic
  proposed by <name|Brélaz> in<nbsp><cite|brelaz>. Obtaining the bounds for
  <math|\<chi\><rsub|G>> is usually very fast, but the difference between the
  bounds grows with <math|<around*|\||V|\|>>.

  Unless the input graph is sparse enough, the algorithm slows down
  considerably for <math|<around*|\||V|\|>\<gtr\>40>.

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

  <subsection|<math|k>-coloring>

  The command <verbatim|is_vertex_colorable> is used for determining whether
  the vertices of a graph can be colored with at most <math|k> colors.

  <verbatim|is_vertex_colorable> accepts two or three arguments: the input
  graph <math|G<around*|(|V,E|)>>, a positive integer <math|k> and optionally
  an unassigned identifier. The command returns <verbatim|true> if <math|G>
  can be colored using at most <math|k> colors and <verbatim|false>
  otherwise. If an identifier is given, a coloring using at most <math|k>
  colors is stored to it as a list of vertex colors, in the order of
  <verbatim|vertices(G)>.

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

  From the results of the last two command lines it follows that
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

  \;

  <section|Edge coloring>

  <chapter|Traversing graphs>

  <section|Walks and tours>

  <subsection|Eulerian graphs>

  The command <verbatim|is_eulerian> is used for determining whether the
  given graph is an Eulerian graph and for finding Eulerian trails in such
  graphs.

  <verbatim|is_eulerian> accepts one or two arguments, the input graph
  <math|G<around*|(|V,E|)>> and optionally an unassigned identifier <math|T>,
  and returns <verbatim|true> if <math|G> is Eulerian and <verbatim|false>
  otherwise. If <math|T> is given, the corresponding Eulerian trail is stored
  to it.

  The graph <math|G> is Eulerian if it has a trail covering all its edges.
  Such a trail is called <em|Eulerian trail>. An Eulerian trail may be
  closed, in which case it is called <em|Eulerian cycle>. Note that every
  edge <math|e\<in\>E> must be visited, i.e.<nbsp>\Pstrolled through\Q,
  exactly once. However, the edge endpoints (i.e.<nbsp>the vertices in
  <math|G>) may be visited more than once.

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
      purge(T):; is_eulerian(complete_graph([1,2,3,4,5]),T); T
    <|unfolded-io>
      <\equation*>
        <math-up|Done>,<text|true>,<around|[|1,2,3,4,1,5,2,4,5,3,1|]>
      </equation*>
    </unfolded-io>
  </session>

  <subsection|Hamiltonian graphs>

  <section|Optimal routing>

  <subsection|Shortest paths in unweighted graphs>

  The command <with|font-family|tt|shortest_path> is used to find the
  shortest path between two vertices in an undirected unweighted graph.

  <with|font-family|tt|shortest_path> accepts three arguments: a graph
  <math|G<around|(|V,E|)>>, the source vertex <math|s\<in\>V> and the target
  vertex <math|t\<in\>V> or a list <math|T> of target vertices. The shortest
  path from source to target is returned. If more targets are specified, the
  list of shortest paths from the source to each of these vertices is
  returned.

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

  <center|<image|images/short.eps|40%|||>>

  <subsection|Cheapest paths in weighted graphs>

  The command <with|font-family|tt|dijkstra> is used to find the cheapest
  path between two vertices of an undirected weighted graph.

  <with|font-family|tt|dijkstra> accepts two or three arguments: a weighted
  graph <math|G<around|(|V,E|)>> with nonnegative weights, a vertex
  <math|s\<in\>V> and optionally a vertex <math|t\<in\>V> or list <math|T> of
  vertices in <math|V>. It returns the cheapest path from <math|s> to
  <math|t> or, if more target vertices are given, the list of such paths to
  each target vertex <math|t\<in\>T>, computed by <name|Dijkstra>'s algorithm
  in <math|O<around|(|<around|\||V|\|><rsup|2>|)>> time. If no target vertex
  is specified, all vertices in <math|V\<setminus\><around|{|s|}>> are
  assumed to be targets.

  A cheapest path from <math|s> to <math|t> is represented with a list
  <with|font-family|tt|[[v1,v2,...,vk],c]> where the first element consists
  of path vertices with <math|v<rsub|1>=s> and <math|v<rsub|k>=t>, while the
  second element <math|c> is the weight (cost) of that path, equal to the sum
  of weights of its edges.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      \;
    </input>
  </session>

  <subsection|Traveling salesman problem>

  <section|Spanning trees>

  <subsection|Constructing a spanning tree>

  The command <with|font-family|tt|spanning_tree> accepts one or two
  arguments, an undirected graph <math|G> and optionally a vertex
  <math|r\<in\>V>. It returns the spanning tree <math|T> of <math|G> rooted
  in <math|r> or, if none is given, in the first vertex in the list <math|V>,
  obtained by depth-first traversal in <math|O<around|(|<around|\||V|\|>+<around|\||E|\|>|)>>
  time.

  <subsection|Minimal spanning tree>

  The command <with|font-family|tt|minimal_spanning_tree> accepts an
  undirected graph <math|G<around|(|V,E|)>> as its only argument and returns
  its minimal spanning tree obtained by <name|Kruskal>'s algorithm in
  <math|O<around|(|<around|\||E|\|>*log <around|\||V|\|>|)>> time.

  <subsection|Counting all spanning trees>

  The command <verbatim|number_of_spanning_trees> is used for counting all
  spanning trees in a graph.

  <verbatim|number_of_spanning_trees> accepts the input graph
  <math|G<around*|(|V,E|)>> as its only argument and returns the total number
  <math|n> of mutually different spanning trees in <math|G>.

  The strategy is based on Theorem<nbsp>2.2.12 (Matrix Tree Theorem)
  in<nbsp><cite|west>, page<nbsp>86. First the adjacency matrix <math|A> and
  the degree sequence <math|\<delta\>> of <math|G> are obtained. Then the
  matrix <math|B=\<Delta\>-A> is computed, where <math|\<Delta\>> is the
  diagonal matrix of order <math|<around*|\||V|\|>> corresponding to
  <math|\<delta\>>. The last row and the last column of <math|B> are popped
  out, yielding the matrix <math|C> of order <math|<around*|\||V|\|>-1>. Now
  <math|n=det C>. In fact, every first minor of <math|B> is equal to
  <math|n>.

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

  <section|Drawing graphs using various methods>

  To visualize a graph use the <kbd|draw_graph> command. It is capable to
  produce a drawing of the given graph using one of the several built-in
  methods.

  <subsection|Overview>

  <with|font-family|tt|draw_graph> accepts one or two arguments, the
  mandatory first one being the graph <math|G<around|(|V,E|)>>. This command
  assigns 2D or 3D coordinates to each vertex <math|v\<in\>V> and produces a
  visual representation of <math|G> based on these coordinates. The second
  (optional) argument is a sequence of options. Each option is one of the
  following:

  <\itemize>
    <item><with|font-family|tt|labels=true> or <with|font-family|tt|false>:
    controls the visibility of vertex labels and edge weights (by default
    <with|font-family|tt|true>, i.e.<nbsp>the labels and weights are
    displayed)

    <item><with|font-family|tt|spring>: draw the graph <math|G> using a
    multilevel force-directed algorithm

    <item><with|font-family|tt|tree[=r> or
    <with|font-family|tt|[r1,r2,...]]>: draw the tree or forest <math|G>,
    optionally specifying root nodes for each tree

    <item><with|font-family|tt|bipartite>: draw the bipartite graph <math|G>
    keeping the vertex partitions separated

    <item><with|font-family|tt|circle[=L]> or
    <with|font-family|tt|convexhull[=L]>: draw the graph <math|G> by setting
    the <with|font-shape|italic|hull vertices> from list <math|L\<subset\>V>
    (assuming <math|L=V> by default) on the unit circle and all other
    vertices in origin, subsequently applying a force-directed vertex
    placement algorithm to generate the layout while keeping the hull
    vertices fixed

    <item><with|font-family|tt|planar> or <with|font-family|tt|plane>: draw
    the planar graph <math|G> using a force-directed algorithm

    <item><with|font-family|tt|plot3d>: draw the connected graph <math|G> as
    if the <with|font-family|tt|spring> option was enabled, but with vertex
    positions in 3D instead of 2D

    <item>any unassigned identifier <math|P>: when given, the vertex
    coordinates will be stored to it in form of a list
  </itemize>

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

  <center|<image|images/sparse.eps|35%|||>>

  <subsection|Spring method>

  When the option <kbd|spring> is specified, the input graph is drawn using
  the force-directed algorithm described in<nbsp><cite|hu> (for an example of
  such a drawing see Figure<nbsp><reference|st53>). The idea, originally due
  to <name|Fruchterman> and <name|Reingold> <cite|fruchterman>, is to
  simulate physical forces in a spring-electrical model where the vertices
  and edges represent equally charged particles and springs connecting them,
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
  the local energy minimum (ideally, the vertex positions should settle in
  the global minimal energy constellation). To avoid this a <em|multilevel>
  scheme is applied. The input graph is iteratively coarsened, either by
  removing the vertices from a maximal independent vertex set or contracting
  the edges of a maximal matching in each iteration. Each coarsening level is
  then processed by the force-directed algorithm, starting from the deepest
  (coarsest) one and <em|lifting> the obtained layout to the first upper
  level, using it as the initial layout for that level. The lifting is
  achieved by using the prolongation matrix technique described in
  <cite|hu2>. To support drawing of large graphs (with 1000 vertices or
  more), the matrices used in the lifting process are stored as sparse
  matrices. The multilevel algorithm is significantly faster than the
  original, single-level one and usually produces better results.

  Graph layouts obtained by using force-directed method have a unique
  property of reflecting symmetries in the design of the input graph, if any.
  Thus the drawings become more appealing and illustrate the certain
  properties of the input graph better. To make the symmetry more prominent,
  the drawing is rotated such that the axis, with respect to which the layout
  exhibits the largest <em|symmetry score>, becomes vertical. As the symmetry
  detection is in general very computationally expensive\Vup to
  <math|O<around*|(|<around*|\||V|\|><rsup|7>|)>> when using the symmetry
  measure of <name|Purchase> <cite|welch>, for example\Vthe algorithm deals
  only with the convex hull and the barycenter of the layout, which may not
  always be enough to produce the optimal result. Nevertheless, this approach
  is very fast and seems to work most of the time for graphs with a high
  level of symmetry (for example the octahedral graph).

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

  <center|<image|images/tensorproduct.eps|40%|||>>

  The following command lines demonstrate drawing of a much larger graph.

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

  <center|<image|images/sg54.eps|40%|||>>

  Note that vertex labels are automatically suppressed because of the large
  number of vertices. On our system, the algorithm took less than 2 seconds
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

  <center|<image|images/ball.eps|50%|||>>

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

  <center|<image|images/3d.eps|40%|||>>

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

  The algorithm implemented in Giac generally satisfies all the above
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

  For example, the following command line produces the drawing of a random
  tree on 100 nodes.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(random_tree(100))
    </input>
  </session>

  <center|<image|images/tree.eps|40%|||>>

  <subsection|Drawing planar graphs>

  The algorithm implemented in Giac which draws planar graphs uses
  augmentation techniques to extend the input graph <math|G> to a graph
  <math|G<rprime|'>>, which is homeomorphic to some triconnected graph, by
  adding temporary edges. The augmented graph <math|G<rprime|'>> is then
  drawn using <name|Tutte>'s barycentric method <cite|tutte>, a
  force-directed algorithm which puts each vertex in the barycenter of its
  neighbors. It is guaranteed that a (non-strict) convex drawing will be
  produced, without edge crossings. In the end, the duplicate of the outer
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
  <math|G> is decomposed into biconnected components called <em|blocks> using
  the depth-first search (see <cite|gibbons>, page<nbsp>25). Each block is
  then decomposed into faces (represented by cycles of vertices) using
  <name|Demoucron>'s algorithm (see <cite|gibbons>, page<nbsp>88, with a
  correction proposed in<nbsp><cite|myrwold>). Embeddings obtained for each
  blocks are then combined by adding one temporary edge for each articulation
  point, joining the two corresponding blocks.
  Figure<nbsp><reference|blockjoin> shows the outer faces of two blocks
  <math|B<rsub|1>> and <math|B<rsub|2>>, connected by an articulation point
  (cut vertex). The temporary edge (shown in blue) is added to join
  <math|B<rsub|1>> and <math|B<rsub|2>> into a single block. After \Pfolding
  up\Q the tree of blocks, the algorithm picks the largest face in the
  resulting biconnected graph to be the outer face of the planar embedding.

  The second part of the augmentation process consists of recursively
  decomposing each non-convex inner face into several convex polygons by
  adding temporary edges. An inner face <math|f=<around*|(|v<rsub|1>,\<ldots\>,v<rsub|n>|)>>
  is non-convex if there exist <math|k> and <math|l> such that
  <math|1\<leqslant\>k\<less\>l-1\<less\>n> and either <math|v<rsub|k>
  v<rsub|l>\<in\>E>, in which case the edge <math|v<rsub|k> v<rsub|l>> is a
  <em|chord> (see Figure<nbsp><reference|chordface> for an example) or there
  exists a face <math|g=<around*|(|w<rsub|1>,w<rsub|2>,\<ldots\>,v<rsub|k>,\<ldots\>,v<rsub|l>,\<ldots\>,w<rsub|m-1>,w<rsub|m>|)>>
  such that the vertices <math|v<rsub|k+1>,\<ldots\>,v<rsub|l-1>> are not
  contained in <math|g> (see Figure<nbsp><reference|touchface> for an
  example). In Figures<nbsp><reference|blockjoin>, <reference|chordface>
  and<nbsp><reference|touchface>, the temporary edges added by the algorithm
  are drawn in green.<\float|float|tbh>
    <center|<small-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<point|-7.02714|4.71764>|<point|-7.27902|3.87308>|<point|-6.21221|2.99889>|<with|color|green|line-width|2ln|<line|<point|-5.9307|3.7101>|<point|-5.32321|2.76182>>>|<point|-5.9307|3.7101>|<point|-6.36038|4.55465>|<point|-6.74562|2.13952>|<point|-6.0196|1.26533>|<point|-5.11577|0.998627>|<point|-4.83425|2.02098>|<point|-5.32321|2.76182>|<line|<point|-7.02714|4.71764>|<point|-6.36038|4.55465>|<point|-5.9307|3.7101>|<point|-6.21221|2.99889>|<point|-7.27902|3.87308>|<point|-7.02714|4.71764>>|<line|<point|-6.21221|2.99889>|<point|-6.74562|2.13952>|<point|-6.0196|1.26533>|<point|-5.11577|0.998627>|<point|-4.83425|2.02098>|<point|-5.32321|2.76182>|<point|-6.21221|2.99889>>|<text-at|<math|B<rsub|1>>|<point|-6.86415200423336|3.87308175684614>>|<text-at|<math|B<rsub|2>>|<point|-5.93069519777748|1.90245072099484>>|<text-at|temp.<nbsp>edge|<point|-6.02040613837809|4.26236936102659>>|<with|dash-style|11100|<line|<point|-4.92621811732097|4.05921748908586>|<point|-5.44174163249107|3.28041076862019>>>>>|<label|blockjoin>joining
    two blocks><small-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<with|color|green|line-width|2ln|<line|<point|-7.03767|3.2286>|<point|-6.0|3.6>>>|<point|-6|3.6>|<text-at|<math|f>|<point|-5.7|2.8>>|<with|color|red|line-width|2ln|<line|<point|-6.59316708559333|4.25095912157693>|<point|-4.25833443577193|4.01635467654452>>>|<point|-6.59316708559333|4.25095912157693>|<point|-4.91887154385501|2.01362614102394>|<point|-6.51908321206509|2.36922873395952>|<point|-7.0376703267628|3.22860166688715>|<with|color|green|line-width|2ln|<line|<point|-5.22142|3.45332>|<point|-4.18425056224368|2.8902797989152>>>|<point|-5.22142479163911|3.45331723772986>|<line|<point|-6.59316708559333|4.25095912157693>|<point|-7.0376703267628|3.22860166688715>|<point|-6.51908321206509|2.36922873395952>|<point|-4.91887154385501|2.01362614102394>|<point|-4.18425056224368|2.8902797989152>|<point|-4.25833443577193|4.01635467654452>|<point|-5.22142479163911|3.45331723772986>|<point|-6.0|3.6>|<point|-6.59316708559333|4.23614234687128>>|<point|-4.18425056224368|2.8902797989152>|<point|-4.25833443577193|4.01635467654452>|<text-at|chord|<point|-5.82891255457071|5.08316245535124>>|<with|dash-style|11100|<line|<point|-5.36484654054769|4.95510318825241>|<point|-5.3399589892843|4.31269017065749>>>>>|<label|chordface>a
    chorded face><small-figure|<with|gr-mode|<tuple|group-edit|edit-props>|gr-frame|<tuple|scale|1cm|<tuple|0.5gw|0.5gh>>|gr-geometry|<tuple|geometry|1par|0.6par|top>|gr-auto-crop|true|gr-color|light
    grey|gr-line-width|2ln|<graphics||<with|color|green|line-width|2ln|<line|<point|-7.47976|3.22362>|<point|-6.31419|3.16188>>>|<with|color|green|line-width|2ln|<line|<point|-5.73634|3.17669>|<point|-4.96587|4.11015>>>|<with|color|red|<line|<point|-7.12912|2.64329>|<point|-7.47975922741103|3.22362415663448>|<point|-7.61807|3.87308>|<point|-7.04022|4.45094>|<point|-5.95859|4.64355>|<point|-4.96587|4.11015>|<point|-5.14367|2.99889>|<point|-5.73634|3.17669>|<point|-6.31419|3.16188>|<point|-7.12912|2.64329>>>|<with|color|green|line-width|2ln|<line|<point|-7.24765|2.09507>|<point|-6.71425|2.33214>>>|<with|color|green|line-width|2ln|<line|<point|-5.57336|2.53957>|<point|-4.53618|2.33214>>>|<with|color|blue|<line|<point|-7.12912|2.64329>|<point|-7.24765|2.09507>|<point|-7.17357|1.39868>|<point|-6.37346|0.89491>|<point|-5.42519|0.835643>|<point|-4.62508|1.51721>|<point|-4.53618|2.33214>|<point|-5.14367|2.99889>|<point|-5.57336|2.53957>|<point|-6.03268|2.22842>|<point|-6.71425|2.33214>|<point|-7.12912|2.64329>>>|<point|-7.04022|4.45094>|<point|-7.61807|3.87308>|<point|-7.12912|2.64329>|<point|-6.31419|3.16188>|<point|-5.73634|3.17669>|<point|-5.14367|2.99889>|<point|-4.96587|4.11015>|<point|-5.95859|4.64355>|<point|-7.24765|2.09507>|<point|-7.17357|1.39868>|<point|-6.37346|0.89491>|<point|-5.42519|0.835643>|<point|-4.62508|1.51721>|<point|-4.53618|2.33214>|<point|-5.57336|2.53957>|<point|-6.03268|2.22842>|<point|-6.71425|2.33214>|<text-at|<math|f>|<point|-6.37346209816113|3.7397307844953>>|<text-at|<math|g>|<point|-5.82524143405212|1.50239780394232>>|<text-at|<math|v<rsub|l>>|<point|-4.94369294880275|3.06221060986903>>|<text-at|<math|v<rsub|k>>|<point|-7.6551627199365|2.48435639634872>>|<point|-7.47975922741103|3.22362415663448>>>|<label|touchface>two
    adjacent faces>>
  </float>

  This method of drawing planar graphs operates in
  <math|O<around*|(|<around*|\||V|\|><rsup|2>|)>> time. Nevertheless, it is
  quite fast for graphs up to 1000 vertices, usually producing results in
  less than a second. The drawback of this method is that it sometimes
  creates clusters of vertices which are very close to each other, resulting
  in a very high ratio of the area of the largest inner face to the area of
  the smallest inner face. However, if the result is not satisfactory, one
  should simply redraw the graph and repeat the process until a better layout
  is found. The planar embedding will in general be different each time.

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

  <center|<image|images/planar1.eps|40%|||>>

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
      G:=delete_vertex(G,8)
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

  <center|<image|images/planar2.eps|40%|||>>

  In the above example, several redraws were required to obtain a good planar
  embedding.

  <subsection|Circular graph drawings>

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
  Petersen graphs. In addition, if the input graph is planar, the drawing
  will also be planar (there is a possibility, however, that some very short
  edges may cross each other as the number of force update iterations is
  limited).

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

  <center|<image|images/sg24.eps|40%|||>>

  <section|Custom vertex positions>

  <subsection|Setting vertex positions>

  The command <verbatim|set_vertex_positions> is used to assign custom
  coordinates to vertices of a graph to be used when drawing the graph.

  <verbatim|set_vertex_positions> accepts two arguments, the graph
  <math|G<around*|(|V,E|)>> and the list <math|L> of positions to be assigned
  to vertices in order of <verbatim|vertices(G)>. The positions may be
  complex numbers, lists of coordinates or points (geometrical objects
  created with the command <verbatim|point>). <verbatim|set_vertex_positions>
  returns the copy <math|G<rprime|'>> of <math|G> with the given layout
  stored in it.

  Any subsequent call to <verbatim|draw_graph> with <math|G<rprime|'>> as an
  argument and without specifying the drawing style will result in displaying
  vertices at the stored coordinates. However, if a drawing style is
  specified, the stored layout is ignored (although it stays stored in
  <math|G<rprime|'>>).

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

  <center|<image|images/digraph1.eps|40%|||>>

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

  <center|<image|images/digraph2.eps|40%|||>>

  <subsection|Generating vertex positions>

  Vertex positions can be generated for a particular graph <math|G> by using
  the <verbatim|draw_graph> command with the additional argument <math|P>
  which should be an unassigned identifier. After the layout is obtained, it
  will be stored to <math|P> as a list of positions (complex numbers for
  <abbr|2D> drawings or points for <abbr|3D> drawings) for each vertex in
  order of <verbatim|vertices(G)>.

  This feature combines well with the <verbatim|set_vertex_positions>
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

  <center|<image|images/oct2.eps|40%|||>>

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

  <center|<image|images/oct1.eps|40%|||>>

  It should be noted that, after a particular layout is fixed, it stays valid
  when some edges or vertices are removed or when an edge is contracted. The
  stored layout becomes invalid only if a new vertex is added to the graph
  (unless its position is specified by <verbatim|set_vertex_attribute> upon
  the creation) or if the <verbatim|position> attribute of an existing vertex
  is discarded.

  <section|Highlighting parts of a graph>

  <subsection|Highlighting vertices>

  The command <verbatim|highlight_vertex> is used for changing color of one
  or more vertices in a graph.

  <verbatim|highlight_vertex> accepts two or three arguments: the graph
  <math|G<around*|(|V,E|)>>, a vertex <math|v\<in\>V> or a list
  <math|L\<subset\>V> of vertices and optionally the new color (or a list of
  colors) for the selected vertices (the default color is green). It returns
  a modified copy of <math|G> in which the specified vertices are colored
  with the specified color.

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

  <center|<image|images/dh1.eps|40%|||>>

  <subsection|Highlighting edges and trails>

  To highlight an edge or a set of edges in a graph, use the
  <verbatim|highlight_edges> command. If the edges form a trail, it is
  usually more convenient to use the <verbatim|highlight_trail> command (see
  below).

  <verbatim|highlight_edges> accepts two or three arguments: the graph
  <math|G<around*|(|V,E|)>>, an edge <math|e> or a list of edges <math|L> and
  optionally the new color (or a list of colors) for the selected edges (the
  default color is red). It returns a modified copy of <math|G> in which the
  specified edges are colored with the specified color.

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

  <center|<image|images/dh2.eps|40%|||>>

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

  <center|<image|images/dh3.eps|40%|||>>

  <verbatim|highlight_trail> accepts two or three arguments: the input graph
  <math|G<around*|(|V,E|)>>, a list <math|L\<subset\>V> or a list
  <verbatim|[L1,L2,...,Lk]> of such lists (each list represents the vertices
  of the corresponding trail) and optionally a positive integer <math|c> or a
  list of positive integers <verbatim|[c1,c2,...,ck]>. The command returns
  the copy of <math|G> in which edges between consecutive vertices in
  <math|L> are highlighted with color <math|c> (by default red) or the trail
  represented by <math|L<rsub|i>> is highlighted with color <math|c<rsub|i>>
  for <math|i=1,2,\<ldots\>,k>.

  Note that a trail can cross itself, which means that the elements of
  <math|L> are not required to be unique.

  <\session|giac|default>
    <\input>
      \<gtr\>\ 
    <|input>
      draw_graph(highlight_trail(G,[6,15,20,19,18,17,16,11,7,2,3,8,13,9,14,10]))
    </input>
  </session>

  <center|<image|images/dh4.eps|40%|||>>

  <subsection|Highlighting subgraphs>

  The command <verbatim|highlight_subgraph> is used for highlighting
  subgraph(s) of the given graph.

  <verbatim|highlight_subgraph> accepts two or four arguments: the graph
  <math|G<around*|(|V,E|)>>, a subgraph <math|S> of <math|G> or a list of
  subgraphs of <math|G> and optionally the new colors for edges and vertices
  of the selected subgraph(s), respectively. It returns a modified copy of
  <math|G> with the selected subgraph(s) colored as specified.

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

  <center|<image|images/sg1.eps|40%|||>>

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

  <center|<image|images/sg2.eps|40%|||>>

  <\bibliography|bib|tm-plain|graphtheory>
    <\bib-list|24>
      <bibitem*|1><label|bib-brelaz>Daniel Brélaz. <newblock>New Methods to
      Color the Vertices of a Graph. <newblock><with|font-shape|italic|Communications
      of the ACM>, 22:251\U256, 1979.<newblock>

      <bibitem*|2><label|bib-buchheim>Cristoph Buchheim, Michael
      Jünger<localize|, and >Sebastian Leipert. <newblock>Improving Walker's
      Algorithm to Run in Linear Time. <newblock><localize|In
      >M.<nbsp>T.<nbsp>Goodrich<localize| and
      >S.<nbsp>G.<nbsp>Kobourov<localize|, editors>,
      <with|font-shape|italic|Graph Drawing 2002, Lecture Notes in Computer
      Science vol 2528>, <localize|pages >344\U353. Springer-Verlag Berlin
      Heidelberg, 2002.<newblock>

      <bibitem*|3><label|bib-diaz>Isabel<nbsp>M.<nbsp>Díaz<localize| and
      >Paula Zabala. <newblock>A Branch-and-Cut Algorithm for Graph Coloring.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      154:826\U847, 2006.<newblock>

      <bibitem*|4><label|bib-edmonds>Jack Edmonds. <newblock>Paths, Trees,
      and Flowers. <newblock><localize|In >Gessel I.<localize| and
      >GC.<nbsp>Rota<localize|, editors>, <with|font-shape|italic|Classic
      Papers in Combinatorics>, <localize|pages >361\U379. Birkhäuser Boston,
      2009. Modern Birkhäuser Classics.<newblock>

      <bibitem*|5><label|bib-even>Shimon Even<localize| and
      >Robert<nbsp>E.<nbsp>Tarjan. <newblock>Computing an st-numbering.
      <newblock><with|font-shape|italic|Theoretical Computer Science>,
      2:339\U344, 1976.<newblock>

      <bibitem*|6><label|bib-fruchterman>T.<nbsp>M.<nbsp>J.<nbsp>Fruchterman<localize|
      and >E.<nbsp>M.<nbsp>Reingold. <newblock>Graph Drawing by
      Force-Directed Placement. <newblock><with|font-shape|italic|Software:
      Practice and Experience>, 21:1129\U1164, 1991.<newblock>

      <bibitem*|7><label|bib-gibbons>Alan Gibbons.
      <newblock><with|font-shape|italic|Algorithmic graph theory>.
      <newblock>Cambridge University Press, 1985.<newblock>

      <bibitem*|8><label|bib-hierholzer>Carl Hierholzer. <newblock>Ueber die
      möglichkeit, einen Linienzug ohne Wiederholung und ohne Unterbrechung
      zu umfahren. <newblock><with|font-shape|italic|Mathematische Annalen>,
      6:30\U32, 1873.<newblock>

      <bibitem*|9><label|bib-hinz>Andreas<nbsp>M.<nbsp>Hinz, Sandi
      Klavºar<localize|, and >Sara<nbsp>S.<nbsp>Zemlji£. <newblock>A survey
      and classification of Sierpi«ski-type graphs.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      217:565\U600, 2017.<newblock>

      <bibitem*|10><label|bib-hopcroft>John<nbsp>E.<nbsp>Hopcroft<localize|
      and >Richard<nbsp>M.<nbsp>Karp. <newblock>An <math|n<rsup|5/2>>
      algorithm for maximum matchings in bipartite graphs.
      <newblock><with|font-shape|italic|SIAM Journal on Computing>,
      2:225\U231, 1973.<newblock>

      <bibitem*|11><label|bib-hu>Yifan Hu. <newblock>Efficient and High
      Quality Force-Directed Graph Drawing.
      <newblock><with|font-shape|italic|Mathematica Journal>, 10:37\U71,
      2005.<newblock>

      <bibitem*|12><label|bib-hu2>Yifan Hu<localize| and >Jennifer Scott.
      <newblock>A Multilevel Algorithm for Wavefront Reduction.
      <newblock><with|font-shape|italic|SIAM Journal on Scientific
      Computing>, 23:1352\U1375, 2001.<newblock>

      <bibitem*|13><label|bib-kahn>Arthur<nbsp>B.<nbsp>Kahn.
      <newblock>Topological sorting of large networks.
      <newblock><with|font-shape|italic|Communications of the ACM>,
      5:558\U562, 1962.<newblock>

      <bibitem*|14><label|bib-myrwold>Wendy Myrwold<localize| and >Willian
      Kocay. <newblock>Errors in graph embedding algorithms.
      <newblock><with|font-shape|italic|Journal of Computer and System
      Sciences>, 77:430\U438, 2011.<newblock>

      <bibitem*|15><label|bib-ostergard>Patric<nbsp>R.<nbsp>J.<nbsp>Östergård.
      <newblock>A fast algorithm for the maximum clique problem.
      <newblock><with|font-shape|italic|Discrete Applied Mathematics>,
      120:197\U207, 2002.<newblock>

      <bibitem*|16><label|bib-plestenjak>Bor Plestenjak. <newblock>An
      Algorithm for Drawing Planar Graphs.
      <newblock><with|font-shape|italic|Software: Practice and Experience>,
      29:973\U984, 1999.<newblock>

      <bibitem*|17><label|bib-steger>Angelika Steger<localize| and
      >Nicholas<nbsp>C.<nbsp>Wormald. <newblock>Generating random regular
      graphs quickly. <newblock><with|font-shape|italic|Combinatorics
      Probability and Computing>, 8:377\U396, 1999.<newblock>

      <bibitem*|18><label|bib-tarjan72>Robert<nbsp>E.<nbsp>Tarjan.
      <newblock>Depth-First Search and Linear Graph Algorithms.
      <newblock><with|font-shape|italic|SIAM Journal on Computing>,
      1:146\U160, 1972.<newblock>

      <bibitem*|19><label|bib-lca>Robert<nbsp>E.<nbsp>Tarjan.
      <newblock>Applications of path compression on balanced trees.
      <newblock><with|font-shape|italic|Journal of the ACM>, 26:690\U715,
      1979.<newblock>

      <bibitem*|20><label|bib-tomita>Etsuji Tomita, Akira Tanaka<localize|,
      and >Haruhisa Takahashi. <newblock>The worst-case time complexity for
      generating all maximal cliques and computational experiments.
      <newblock><with|font-shape|italic|Theoretical Computer Science>,
      363:28\U42, 2006.<newblock>

      <bibitem*|21><label|bib-tutte>W.<nbsp>T.<nbsp>Tutte. <newblock>How to
      draw a graph. <newblock><with|font-shape|italic|Proceedings of the
      London Mathematical Society>, s3-13:743\U767, 1963.<newblock>

      <bibitem*|22><label|bib-walker>John<nbsp>Q.<nbsp>Walker II. <newblock>A
      nodepositioning algorithm for general trees.
      <newblock><with|font-shape|italic|Software: Practice and Experience>,
      20:685\U705, 1990.<newblock>

      <bibitem*|23><label|bib-welch>E.<nbsp>Welch<localize| and
      >S.<nbsp>Kobourov. <newblock>Measuring Symmetry in Drawings of Graphs.
      <newblock><with|font-shape|italic|Computer Graphics Forum>,
      36:341\U351, 2017.<newblock>

      <bibitem*|24><label|bib-west>Douglas<nbsp>B.<nbsp>West.
      <newblock><with|font-shape|italic|Introduction to Graph Theory>.
      <newblock>Pearson Education, 2002.<newblock>
    </bib-list>
  </bibliography>
</body>

<\initial>
  <\collection>
    <associate|font-base-size|10>
    <associate|page-medium|papyrus>
    <associate|page-type|a4>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|a3|<tuple|A3|78>>
    <associate|auto-1|<tuple|1|7>>
    <associate|auto-10|<tuple|2.1.5|11>>
    <associate|auto-100|<tuple|5.2.6|53>>
    <associate|auto-101|<tuple|5.2.7|54>>
    <associate|auto-102|<tuple|5.3|54>>
    <associate|auto-103|<tuple|5.3.1|54>>
    <associate|auto-104|<tuple|5.3.2|55>>
    <associate|auto-105|<tuple|5.3.3|56>>
    <associate|auto-106|<tuple|5.3.4|56>>
    <associate|auto-107|<tuple|5.3.5|56>>
    <associate|auto-108|<tuple|5.3.6|57>>
    <associate|auto-109|<tuple|5.4|57>>
    <associate|auto-11|<tuple|2.1.6|11>>
    <associate|auto-110|<tuple|5.4.1|57>>
    <associate|auto-111|<tuple|5.4.2|58>>
    <associate|auto-112|<tuple|5.4.3|58>>
    <associate|auto-113|<tuple|5.4.4|58>>
    <associate|auto-114|<tuple|5.5|59>>
    <associate|auto-115|<tuple|5.5.1|59>>
    <associate|auto-116|<tuple|5.5.2|59>>
    <associate|auto-117|<tuple|5.5.3|59>>
    <associate|auto-118|<tuple|5.5.4|59>>
    <associate|auto-119|<tuple|5.6|60>>
    <associate|auto-12|<tuple|2.2|12>>
    <associate|auto-120|<tuple|5.6.1|60>>
    <associate|auto-121|<tuple|5.6.2|60>>
    <associate|auto-122|<tuple|5.6.3|60>>
    <associate|auto-123|<tuple|5.7|61>>
    <associate|auto-124|<tuple|5.7.1|61>>
    <associate|auto-125|<tuple|5.7.2|61>>
    <associate|auto-126|<tuple|5.8|62>>
    <associate|auto-127|<tuple|5.8.1|62>>
    <associate|auto-128|<tuple|5.8.2|62>>
    <associate|auto-129|<tuple|5.8.3|63>>
    <associate|auto-13|<tuple|2.2.1|12>>
    <associate|auto-130|<tuple|5.8.4|63>>
    <associate|auto-131|<tuple|5.8.5|64>>
    <associate|auto-132|<tuple|5.9|65>>
    <associate|auto-133|<tuple|5.9.1|65>>
    <associate|auto-134|<tuple|5.1|66>>
    <associate|auto-135|<tuple|5.9.2|66>>
    <associate|auto-136|<tuple|5.2|68>>
    <associate|auto-137|<tuple|5.9.3|68>>
    <associate|auto-138|<tuple|5.9.4|68>>
    <associate|auto-139|<tuple|5.10|69>>
    <associate|auto-14|<tuple|2.2.2|12>>
    <associate|auto-140|<tuple|6|71>>
    <associate|auto-141|<tuple|6.1|71>>
    <associate|auto-142|<tuple|6.1.1|71>>
    <associate|auto-143|<tuple|6.1.2|71>>
    <associate|auto-144|<tuple|6.2|71>>
    <associate|auto-145|<tuple|6.2.1|71>>
    <associate|auto-146|<tuple|6.2.2|72>>
    <associate|auto-147|<tuple|6.2.3|72>>
    <associate|auto-148|<tuple|6.3|72>>
    <associate|auto-149|<tuple|6.3.1|72>>
    <associate|auto-15|<tuple|2.3|12>>
    <associate|auto-150|<tuple|6.3.2|72>>
    <associate|auto-151|<tuple|6.3.3|72>>
    <associate|auto-152|<tuple|7|75>>
    <associate|auto-153|<tuple|7.1|75>>
    <associate|auto-154|<tuple|7.1.1|75>>
    <associate|auto-155|<tuple|7.1.2|75>>
    <associate|auto-156|<tuple|7.1.3|76>>
    <associate|auto-157|<tuple|7.1.4|78>>
    <associate|auto-158|<tuple|7.1.5|79>>
    <associate|auto-159|<tuple|7.1|79>>
    <associate|auto-16|<tuple|2.3.1|12>>
    <associate|auto-160|<tuple|7.2|79>>
    <associate|auto-161|<tuple|7.3|79>>
    <associate|auto-162|<tuple|7.1.6|80>>
    <associate|auto-163|<tuple|7.2|81>>
    <associate|auto-164|<tuple|7.2.1|81>>
    <associate|auto-165|<tuple|7.2.2|81>>
    <associate|auto-166|<tuple|7.3|82>>
    <associate|auto-167|<tuple|7.3.1|82>>
    <associate|auto-168|<tuple|7.3.2|83>>
    <associate|auto-169|<tuple|7.3.3|84>>
    <associate|auto-17|<tuple|2.3.2|13>>
    <associate|auto-170|<tuple|7.3.3|85>>
    <associate|auto-18|<tuple|2.3.3|13>>
    <associate|auto-19|<tuple|2.4|13>>
    <associate|auto-2|<tuple|1.1|7>>
    <associate|auto-20|<tuple|2.4.1|13>>
    <associate|auto-21|<tuple|2.4.2|14>>
    <associate|auto-22|<tuple|2.5|14>>
    <associate|auto-23|<tuple|2.5.1|14>>
    <associate|auto-24|<tuple|2.5.2|15>>
    <associate|auto-25|<tuple|2.6|15>>
    <associate|auto-26|<tuple|2.6.1|15>>
    <associate|auto-27|<tuple|2.6.2|15>>
    <associate|auto-28|<tuple|2.7|16>>
    <associate|auto-29|<tuple|2.7.1|16>>
    <associate|auto-3|<tuple|1.2|7>>
    <associate|auto-30|<tuple|2.7.2|16>>
    <associate|auto-31|<tuple|2.7.3|17>>
    <associate|auto-32|<tuple|2.7.4|17>>
    <associate|auto-33|<tuple|2.7.5|18>>
    <associate|auto-34|<tuple|2.7.6|18>>
    <associate|auto-35|<tuple|2.7.7|18>>
    <associate|auto-36|<tuple|2.7.8|19>>
    <associate|auto-37|<tuple|2.7.9|20>>
    <associate|auto-38|<tuple|2.7.10|21>>
    <associate|auto-39|<tuple|2.8|22>>
    <associate|auto-4|<tuple|2|9>>
    <associate|auto-40|<tuple|2.8.1|22>>
    <associate|auto-41|<tuple|2.8.2|22>>
    <associate|auto-42|<tuple|2.8.3|22>>
    <associate|auto-43|<tuple|2.9|22>>
    <associate|auto-44|<tuple|2.9.1|22>>
    <associate|auto-45|<tuple|2.9.2|23>>
    <associate|auto-46|<tuple|2.9.3|23>>
    <associate|auto-47|<tuple|2.10|23>>
    <associate|auto-48|<tuple|2.10.1|23>>
    <associate|auto-49|<tuple|2.10.2|24>>
    <associate|auto-5|<tuple|2.1|9>>
    <associate|auto-50|<tuple|2.10.3|24>>
    <associate|auto-51|<tuple|2.10.4|25>>
    <associate|auto-52|<tuple|2.10.5|25>>
    <associate|auto-53|<tuple|2.10.6|25>>
    <associate|auto-54|<tuple|2.10.7|26>>
    <associate|auto-55|<tuple|2.10.8|27>>
    <associate|auto-56|<tuple|2.10.9|28>>
    <associate|auto-57|<tuple|2.10.10|29>>
    <associate|auto-58|<tuple|2.10.11|29>>
    <associate|auto-59|<tuple|2.11|30>>
    <associate|auto-6|<tuple|2.1.1|9>>
    <associate|auto-60|<tuple|2.11.1|30>>
    <associate|auto-61|<tuple|2.11.2|31>>
    <associate|auto-62|<tuple|2.11.3|32>>
    <associate|auto-63|<tuple|2.11.4|32>>
    <associate|auto-64|<tuple|2.11.5|34>>
    <associate|auto-65|<tuple|2.11.6|34>>
    <associate|auto-66|<tuple|2.11.7|35>>
    <associate|auto-67|<tuple|2.11.8|35>>
    <associate|auto-68|<tuple|3|37>>
    <associate|auto-69|<tuple|3.1|37>>
    <associate|auto-7|<tuple|2.1.2|10>>
    <associate|auto-70|<tuple|3.1.1|37>>
    <associate|auto-71|<tuple|3.2|37>>
    <associate|auto-72|<tuple|3.2.1|37>>
    <associate|auto-73|<tuple|3.2.2|38>>
    <associate|auto-74|<tuple|3.2.3|38>>
    <associate|auto-75|<tuple|3.2.4|39>>
    <associate|auto-76|<tuple|3.3|39>>
    <associate|auto-77|<tuple|3.3.1|39>>
    <associate|auto-78|<tuple|3.3.2|40>>
    <associate|auto-79|<tuple|3.3.3|41>>
    <associate|auto-8|<tuple|2.1.3|10>>
    <associate|auto-80|<tuple|4|43>>
    <associate|auto-81|<tuple|4.1|43>>
    <associate|auto-82|<tuple|4.1.1|43>>
    <associate|auto-83|<tuple|4.1.2|43>>
    <associate|auto-84|<tuple|4.2|44>>
    <associate|auto-85|<tuple|4.2.1|44>>
    <associate|auto-86|<tuple|4.2.2|44>>
    <associate|auto-87|<tuple|4.1|45>>
    <associate|auto-88|<tuple|5|47>>
    <associate|auto-89|<tuple|5.1|47>>
    <associate|auto-9|<tuple|2.1.4|10>>
    <associate|auto-90|<tuple|5.1.1|47>>
    <associate|auto-91|<tuple|5.1.2|47>>
    <associate|auto-92|<tuple|5.1.3|48>>
    <associate|auto-93|<tuple|5.1.4|49>>
    <associate|auto-94|<tuple|5.2|50>>
    <associate|auto-95|<tuple|5.2.1|50>>
    <associate|auto-96|<tuple|5.2.2|51>>
    <associate|auto-97|<tuple|5.2.3|51>>
    <associate|auto-98|<tuple|5.2.4|52>>
    <associate|auto-99|<tuple|5.2.5|53>>
    <associate|bib-brelaz|<tuple|1|85>>
    <associate|bib-buchheim|<tuple|2|85>>
    <associate|bib-diaz|<tuple|3|85>>
    <associate|bib-edmonds|<tuple|4|85>>
    <associate|bib-even|<tuple|5|85>>
    <associate|bib-fruchterman|<tuple|6|85>>
    <associate|bib-gibbons|<tuple|7|85>>
    <associate|bib-hierholzer|<tuple|8|85>>
    <associate|bib-hinz|<tuple|9|85>>
    <associate|bib-hopcroft|<tuple|10|85>>
    <associate|bib-hu|<tuple|11|85>>
    <associate|bib-hu2|<tuple|12|85>>
    <associate|bib-kahn|<tuple|13|85>>
    <associate|bib-lca|<tuple|19|85>>
    <associate|bib-myrwold|<tuple|14|85>>
    <associate|bib-ostergard|<tuple|15|85>>
    <associate|bib-plestenjak|<tuple|16|85>>
    <associate|bib-steger|<tuple|17|85>>
    <associate|bib-tarjan72|<tuple|18|85>>
    <associate|bib-tomita|<tuple|20|85>>
    <associate|bib-tutte|<tuple|21|85>>
    <associate|bib-walker|<tuple|22|85>>
    <associate|bib-welch|<tuple|23|85>>
    <associate|bib-west|<tuple|24|85>>
    <associate|blockjoin|<tuple|7.1|79>>
    <associate|chordface|<tuple|7.2|79>>
    <associate|footnote-2.1|<tuple|2.1|20>>
    <associate|footnote-2.2|<tuple|2.2|21>>
    <associate|footnote-2.3|<tuple|2.3|30>>
    <associate|footnote-2.4|<tuple|2.4|34>>
    <associate|footnote-4.1|<tuple|4.1|43>>
    <associate|footnote-4.2|<tuple|4.2|43>>
    <associate|footnote-4.3|<tuple|4.3|43>>
    <associate|footnote-4.4|<tuple|4.4|45>>
    <associate|footnote-5.1|<tuple|5.1|61>>
    <associate|footnote-5.2|<tuple|5.2|66>>
    <associate|footnr-2.1|<tuple|2.1|20>>
    <associate|footnr-2.2|<tuple|2.2|21>>
    <associate|footnr-2.3|<tuple|2.3|30>>
    <associate|footnr-2.4|<tuple|2.4|34>>
    <associate|footnr-4.1|<tuple|4.1|43>>
    <associate|footnr-4.2|<tuple|4.2|43>>
    <associate|footnr-4.3|<tuple|4.3|43>>
    <associate|footnr-4.4|<tuple|4.4|45>>
    <associate|footnr-5.1|<tuple|5.1|61>>
    <associate|footnr-5.2|<tuple|5.2|66>>
    <associate|st53|<tuple|4.1|45>>
    <associate|tab:colors|<tuple|5.1|66>>
    <associate|tab:times|<tuple|5.2|68>>
    <associate|touchface|<tuple|7.3|79>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|bib>
      hinz

      tomita

      steger

      gibbons

      tarjan72

      tarjan72

      gibbons

      tarjan72

      lca

      kahn

      even

      edmonds

      hopcroft

      tomita

      ostergard

      diaz

      brelaz

      brelaz

      brelaz

      hierholzer

      west

      hu

      fruchterman

      hu2

      welch

      walker

      buchheim

      tutte

      gibbons

      gibbons

      myrwold

      plestenjak
    </associate>
    <\associate|figure>
      <tuple|normal|<surround|<hidden|<tuple>>||drawing of the Sierpi«ski
      graph <with|mode|<quote|math>|S T<rsub|3><rsup|5>> using
      L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|<quote|0>|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
      and PSTricks>|<pageref|auto-87>>

      <tuple|normal|<surround|<hidden|<tuple>>||joining two
      blocks>|<pageref|auto-159>>

      <tuple|normal|<surround|<hidden|<tuple>>||a chorded
      face>|<pageref|auto-160>>

      <tuple|normal|<surround|<hidden|<tuple>>||two adjacent
      faces>|<pageref|auto-161>>
    </associate>
    <\associate|table>
      <tuple|normal|<surround|<hidden|<tuple>>||interpretation of abstract
      vertex/edge colors in Xcas>|<pageref|auto-134>>

      <tuple|normal|<surround|<hidden|<tuple>>||average runtime of the
      algorithm for minimal coloring (in seconds)>|<pageref|auto-136>>
    </associate>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|1.<space|2spc>Introduction>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-1><vspace|0.5fn>

      1.1.<space|2spc>Overview <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2>

      1.2.<space|2spc>Notations used in this manual
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|2.<space|2spc>Constructing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-4><vspace|0.5fn>

      2.1.<space|2spc>General graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-5>

      <with|par-left|<quote|1tab>|2.1.1.<space|2spc>Creating undirected
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-6>>

      <with|par-left|<quote|1tab>|2.1.2.<space|2spc>Creating directed graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-7>>

      <with|par-left|<quote|1tab>|2.1.3.<space|2spc>Creating vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <with|par-left|<quote|1tab>|2.1.4.<space|2spc>Creating edges and arcs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9>>

      <with|par-left|<quote|1tab>|2.1.5.<space|2spc>Creating paths and trails
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-10>>

      <with|par-left|<quote|1tab>|2.1.6.<space|2spc>Specifying adjacency or
      weight matrix <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-11>>

      2.2.<space|2spc>Promoting to directed and undirected graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-12>

      <with|par-left|<quote|1tab>|2.2.1.<space|2spc>Converting edges to pairs
      of arcs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-13>>

      <with|par-left|<quote|1tab>|2.2.2.<space|2spc>Assigning weight matrix
      to an unweighted graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-14>>

      2.3.<space|2spc>Cycle and path graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-15>

      <with|par-left|<quote|1tab>|2.3.1.<space|2spc>Cycle graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-16>>

      <with|par-left|<quote|1tab>|2.3.2.<space|2spc>Path graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-17>>

      <with|par-left|<quote|1tab>|2.3.3.<space|2spc>Trails of edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-18>>

      2.4.<space|2spc>Complete graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-19>

      <with|par-left|<quote|1tab>|2.4.1.<space|2spc>Complete graphs with
      multiple vertex partitions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-20>>

      <with|par-left|<quote|1tab>|2.4.2.<space|2spc>Complete trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-21>>

      2.5.<space|2spc>Sequence graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-22>

      <with|par-left|<quote|1tab>|2.5.1.<space|2spc>Creating graphs from
      degree sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-23>>

      <with|par-left|<quote|1tab>|2.5.2.<space|2spc>Validating graphic
      sequences <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-24>>

      2.6.<space|2spc>Intersection graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-25>

      <with|par-left|<quote|1tab>|2.6.1.<space|2spc>Interval graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-26>>

      <with|par-left|<quote|1tab>|2.6.2.<space|2spc>Kneser graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-27>>

      2.7.<space|2spc>Special graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-28>

      <with|par-left|<quote|1tab>|2.7.1.<space|2spc>Hypercube graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-29>>

      <with|par-left|<quote|1tab>|2.7.2.<space|2spc>Star graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-30>>

      <with|par-left|<quote|1tab>|2.7.3.<space|2spc>Wheel graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-31>>

      <with|par-left|<quote|1tab>|2.7.4.<space|2spc>Web graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-32>>

      <with|par-left|<quote|1tab>|2.7.5.<space|2spc>Prism graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-33>>

      <with|par-left|<quote|1tab>|2.7.6.<space|2spc>Antiprism graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-34>>

      <with|par-left|<quote|1tab>|2.7.7.<space|2spc>Grid graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-35>>

      <with|par-left|<quote|1tab>|2.7.8.<space|2spc>Sierpi«ski graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-36>>

      <with|par-left|<quote|1tab>|2.7.9.<space|2spc>Generalized Petersen
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-37>>

      <with|par-left|<quote|1tab>|2.7.10.<space|2spc><rigid|LCF> graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-38>>

      2.8.<space|2spc>Isomorphic copies of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-39>

      <with|par-left|<quote|1tab>|2.8.1.<space|2spc>Creating an isomorphic
      copy of a graph <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-40>>

      <with|par-left|<quote|1tab>|2.8.2.<space|2spc>Permuting graph vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-41>>

      <with|par-left|<quote|1tab>|2.8.3.<space|2spc>Relabeling graph vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-42>>

      2.9.<space|2spc>Subgraphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-43>

      <with|par-left|<quote|1tab>|2.9.1.<space|2spc>Extracting subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-44>>

      <with|par-left|<quote|1tab>|2.9.2.<space|2spc>Induced subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-45>>

      <with|par-left|<quote|1tab>|2.9.3.<space|2spc>Underlying graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-46>>

      2.10.<space|2spc>Operations on graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-47>

      <with|par-left|<quote|1tab>|2.10.1.<space|2spc>Graph complement
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-48>>

      <with|par-left|<quote|1tab>|2.10.2.<space|2spc>Seidel switching
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-49>>

      <with|par-left|<quote|1tab>|2.10.3.<space|2spc>Transposing graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-50>>

      <with|par-left|<quote|1tab>|2.10.4.<space|2spc>Union of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-51>>

      <with|par-left|<quote|1tab>|2.10.5.<space|2spc>Disjoint union of graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-52>>

      <with|par-left|<quote|1tab>|2.10.6.<space|2spc>Joining two graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-53>>

      <with|par-left|<quote|1tab>|2.10.7.<space|2spc>Power graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-54>>

      <with|par-left|<quote|1tab>|2.10.8.<space|2spc>Graph products
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-55>>

      <with|par-left|<quote|1tab>|2.10.9.<space|2spc>Transitive closure graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-56>>

      <with|par-left|<quote|1tab>|2.10.10.<space|2spc>Line graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-57>>

      <with|par-left|<quote|1tab>|2.10.11.<space|2spc>Plane dual graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-58>>

      2.11.<space|2spc>Random graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-59>

      <with|par-left|<quote|1tab>|2.11.1.<space|2spc>Random general graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-60>>

      <with|par-left|<quote|1tab>|2.11.2.<space|2spc>Random bipartite graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-61>>

      <with|par-left|<quote|1tab>|2.11.3.<space|2spc>Random trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-62>>

      <with|par-left|<quote|1tab>|2.11.4.<space|2spc>Random planar graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-63>>

      <with|par-left|<quote|1tab>|2.11.5.<space|2spc>Random regular graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-64>>

      <with|par-left|<quote|1tab>|2.11.6.<space|2spc>Random tournaments
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-65>>

      <with|par-left|<quote|1tab>|2.11.7.<space|2spc>Random flow networks
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-66>>

      <with|par-left|<quote|1tab>|2.11.8.<space|2spc>Randomizing edge weights
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-67>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|3.<space|2spc>Modifying
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-68><vspace|0.5fn>

      3.1.<space|2spc>Modifying vertices of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-69>

      <with|par-left|<quote|1tab>|3.1.1.<space|2spc>Adding and removing
      single vertices <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-70>>

      3.2.<space|2spc>Modifying edges of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-71>

      <with|par-left|<quote|1tab>|3.2.1.<space|2spc>Adding and removing
      single edges <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-72>>

      <with|par-left|<quote|1tab>|3.2.2.<space|2spc>Accessing and modifying
      edge weights <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-73>>

      <with|par-left|<quote|1tab>|3.2.3.<space|2spc>Contracting edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-74>>

      <with|par-left|<quote|1tab>|3.2.4.<space|2spc>Subdividing edges
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-75>>

      3.3.<space|2spc>Using attributes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-76>

      <with|par-left|<quote|1tab>|3.3.1.<space|2spc>Graph attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-77>>

      <with|par-left|<quote|1tab>|3.3.2.<space|2spc>Vertex attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-78>>

      <with|par-left|<quote|1tab>|3.3.3.<space|2spc>Edge attributes
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-79>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|4.<space|2spc>Import
      and export> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-80><vspace|0.5fn>

      4.1.<space|2spc>Importing graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-81>

      <with|par-left|<quote|1tab>|4.1.1.<space|2spc>Loading graphs from
      <with|font-family|<quote|ss>|dot> files
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-82>>

      <with|par-left|<quote|1tab>|4.1.2.<space|2spc>The
      <with|font-family|<quote|ss>|dot> file format overview
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-83>>

      4.2.<space|2spc>Exporting graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-84>

      <with|par-left|<quote|1tab>|4.2.1.<space|2spc>Saving graphs in
      <with|font-family|<quote|ss>|dot> format
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-85>>

      <with|par-left|<quote|1tab>|4.2.2.<space|2spc>Saving graph drawings in
      L<rsup|<space|-0.8spc><move|A|0fn|-0.1fn>><space|-0.2spc>T<rsub|<space|-0.4spc><move|<resize|<with|math-level|<quote|0>|E>||||0.5fn>|0fn|-0.1fn>><space|-0.4spc>X
      format <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-86>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|5.<space|2spc>Graph
      properties> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-88><vspace|0.5fn>

      5.1.<space|2spc>Basic properties <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-89>

      <with|par-left|<quote|1tab>|5.1.1.<space|2spc>Listing vertices and
      edges <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-90>>

      <with|par-left|<quote|1tab>|5.1.2.<space|2spc>Vertex degrees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-91>>

      <with|par-left|<quote|1tab>|5.1.3.<space|2spc>Vertex adjacency
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-92>>

      <with|par-left|<quote|1tab>|5.1.4.<space|2spc>Edge incidence
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-93>>

      5.2.<space|2spc>Algebraic properties
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-94>

      <with|par-left|<quote|1tab>|5.2.1.<space|2spc>Adjacency matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-95>>

      <with|par-left|<quote|1tab>|5.2.2.<space|2spc>Weight matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-96>>

      <with|par-left|<quote|1tab>|5.2.3.<space|2spc>Incidence matrix
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-97>>

      <with|par-left|<quote|1tab>|5.2.4.<space|2spc>Characteristic polynomial
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-98>>

      <with|par-left|<quote|1tab>|5.2.5.<space|2spc>Graph spectrum
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-99>>

      <with|par-left|<quote|1tab>|5.2.6.<space|2spc>Seidel spectrum
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-100>>

      <with|par-left|<quote|1tab>|5.2.7.<space|2spc>Integer graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-101>>

      5.3.<space|2spc>Connectivity <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-102>

      <with|par-left|<quote|1tab>|5.3.1.<space|2spc>Vertex connectivity
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-103>>

      <with|par-left|<quote|1tab>|5.3.2.<space|2spc>Connected and biconnected
      components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-104>>

      <with|par-left|<quote|1tab>|5.3.3.<space|2spc>Graph rank
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-105>>

      <with|par-left|<quote|1tab>|5.3.4.<space|2spc>Articulation points
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-106>>

      <with|par-left|<quote|1tab>|5.3.5.<space|2spc>Strongly connected
      components <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-107>>

      <with|par-left|<quote|1tab>|5.3.6.<space|2spc>Edge connectivity
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-108>>

      5.4.<space|2spc>Trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-109>

      <with|par-left|<quote|1tab>|5.4.1.<space|2spc>Tree graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-110>>

      <with|par-left|<quote|1tab>|5.4.2.<space|2spc>Forest graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-111>>

      <with|par-left|<quote|1tab>|5.4.3.<space|2spc>Height of a tree
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-112>>

      <with|par-left|<quote|1tab>|5.4.4.<space|2spc>Lowest common ancestor of
      a pair of nodes <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-113>>

      5.5.<space|2spc>Distance <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-114>

      <with|par-left|<quote|1tab>|5.5.1.<space|2spc>Vertex distance
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-115>>

      <with|par-left|<quote|1tab>|5.5.2.<space|2spc>All-pairs vertex distance
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-116>>

      <with|par-left|<quote|1tab>|5.5.3.<space|2spc>Diameter of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-117>>

      <with|par-left|<quote|1tab>|5.5.4.<space|2spc>Girth of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-118>>

      5.6.<space|2spc>Acyclic graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-119>

      <with|par-left|<quote|1tab>|5.6.1.<space|2spc>Checking if a graph is
      acyclic <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-120>>

      <with|par-left|<quote|1tab>|5.6.2.<space|2spc>Topological sorting
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-121>>

      <with|par-left|<quote|1tab>|5.6.3.<space|2spc><with|font-family|<quote|ss>|st>
      ordering <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-122>>

      5.7.<space|2spc>Vertex matching <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-123>

      <with|par-left|<quote|1tab>|5.7.1.<space|2spc>Maximum matching
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-124>>

      <with|par-left|<quote|1tab>|5.7.2.<space|2spc>Maximum matching in
      bipartite graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-125>>

      5.8.<space|2spc>Cliques <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-126>

      <with|par-left|<quote|1tab>|5.8.1.<space|2spc>Clique graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-127>>

      <with|par-left|<quote|1tab>|5.8.2.<space|2spc>Triangle-free graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-128>>

      <with|par-left|<quote|1tab>|5.8.3.<space|2spc>Maximal cliques
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-129>>

      <with|par-left|<quote|1tab>|5.8.4.<space|2spc>Maximum clique
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-130>>

      <with|par-left|<quote|1tab>|5.8.5.<space|2spc>Minimum clique cover
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-131>>

      5.9.<space|2spc>Vertex coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-132>

      <with|par-left|<quote|1tab>|5.9.1.<space|2spc>Greedy coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-133>>

      <with|par-left|<quote|1tab>|5.9.2.<space|2spc>Minimal coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-135>>

      <with|par-left|<quote|1tab>|5.9.3.<space|2spc>Chromatic number
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-137>>

      <with|par-left|<quote|1tab>|5.9.4.<space|2spc><with|mode|<quote|math>|k>-coloring
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-138>>

      5.10.<space|2spc>Edge coloring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-139>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|6.<space|2spc>Traversing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-140><vspace|0.5fn>

      6.1.<space|2spc>Walks and tours <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-141>

      <with|par-left|<quote|1tab>|6.1.1.<space|2spc>Eulerian graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-142>>

      <with|par-left|<quote|1tab>|6.1.2.<space|2spc>Hamiltonian graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-143>>

      6.2.<space|2spc>Optimal routing <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-144>

      <with|par-left|<quote|1tab>|6.2.1.<space|2spc>Shortest paths in
      unweighted graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-145>>

      <with|par-left|<quote|1tab>|6.2.2.<space|2spc>Cheapest paths in
      weighted graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-146>>

      <with|par-left|<quote|1tab>|6.2.3.<space|2spc>Traveling salesman
      problem <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-147>>

      6.3.<space|2spc>Spanning trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-148>

      <with|par-left|<quote|1tab>|6.3.1.<space|2spc>Constructing a spanning
      tree <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-149>>

      <with|par-left|<quote|1tab>|6.3.2.<space|2spc>Minimal spanning tree
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-150>>

      <with|par-left|<quote|1tab>|6.3.3.<space|2spc>Counting all spanning
      trees <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-151>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|7.<space|2spc>Visualizing
      graphs> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-152><vspace|0.5fn>

      7.1.<space|2spc>Drawing graphs using various methods
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-153>

      <with|par-left|<quote|1tab>|7.1.1.<space|2spc>Overview
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-154>>

      <with|par-left|<quote|1tab>|7.1.2.<space|2spc>Drawing disconnected
      graphs <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-155>>

      <with|par-left|<quote|1tab>|7.1.3.<space|2spc>Spring method
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-156>>

      <with|par-left|<quote|1tab>|7.1.4.<space|2spc>Drawing trees
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-157>>

      <with|par-left|<quote|1tab>|7.1.5.<space|2spc>Drawing planar graphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-158>>

      <with|par-left|<quote|1tab>|7.1.6.<space|2spc>Circular graph drawings
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-162>>

      7.2.<space|2spc>Custom vertex positions
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-163>

      <with|par-left|<quote|1tab>|7.2.1.<space|2spc>Setting vertex positions
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-164>>

      <with|par-left|<quote|1tab>|7.2.2.<space|2spc>Generating vertex
      positions <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-165>>

      7.3.<space|2spc>Highlighting parts of a graph
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-166>

      <with|par-left|<quote|1tab>|7.3.1.<space|2spc>Highlighting vertices
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-167>>

      <with|par-left|<quote|1tab>|7.3.2.<space|2spc>Highlighting edges and
      trails <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-168>>

      <with|par-left|<quote|1tab>|7.3.3.<space|2spc>Highlighting subgraphs
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-169>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|font-shape|<quote|small-caps>|Bibliography>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <pageref|auto-170><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>