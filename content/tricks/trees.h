1. Given a bridge tree,
    the minimum number of edges required to be added to make the graph 2 -
        edge
            connected = ceil(L / 2) where L are the number of leaves
                            .2. Block cut tree builds a bipartite
                            graph(original node->block node
                                      ->original node(may be articulation point)
                                      ->...)
                            .Non -
                        articulation points have degree = 1,
            and articulation points have degree >=
                2(connecting two or more graphs).
