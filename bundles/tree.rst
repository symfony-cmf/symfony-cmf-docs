TreeBundle
==========

The `TreeBundle <https://github.com/symfony-cmf/TreeBundle#readme>`_
provides integration with jQuery tree.

.. index:: TreeBundle

Dependencies
------------

* Install jQuery. `SonatajQueryBundle <https://github.com/sonata-project/SonatajQueryBundle>`_ strongly suggested.

Configuration
-------------

There is no configuration for this bundle.

Features
--------

 * Nodes expanding/collapsing.
 * Subtrees lazy loading via JSON replies to AJAX calls.
 * Callback function when a node is toggled.

Usage
-----

 * Include CSS & JS files in your template.
 * Call ``$("#tree").jstree({/* parameters */})`` - assuming here *#tree* is the selector of your list.
 * Provide *url* value pointing to a server-side-something returning lists of children for any given node ID.

Server must reply in JSON format, this is an example:

.. code-block:: javascript

    [
        {"data":"root","attr":{"id":"root","rel":"folder"},"state":"closed","children":
            [
                {"data":"content","attr":{"id":"child1","rel":"folder"},"state":"closed"},
                {"data":"menu","attr":{"id":"child2","rel":"folder"},"state":"closed"},
                {"data":"routes","attr":{"id":"child3","rel":"folder"},"state":"closed"}
            ]
        }
    ]
```

More info on setup available on `jsTree's website <http://www.jstree.com/documentation>`_.

Example HTML
------------

.. code-block:: html

    <html>
        <head>
            <title>CMF Sandbox - Treeview test</title>

            <link href="/bundles/symfonycmftree/css/jquery.treeview.css" media="screen" type="text/css" rel="stylesheet" />

            <script src="/bundles/sonatajquery/jquery-1.7.1.js" type="text/javascript"></script>

            <script src="{{ asset('bundles/symfonycmftree/js/jstree/jquery.jstree.js') }}" type="text/javascript"></script>

            <script type="text/javascript">
                function initTrees() {

                    jQuery("#tree").jstree({
                        "plugins" :     [ "themes", "types", "ui", "json_data" ],
                        "json_data": {
                            "ajax": {
                                url:    "subtree.php",
                                data:   function (node) {
                                    return { 'root' : jQuery(node).attr('id') };
                                }
                            }
                        },
                        "types": {
                            "max_depth":        -2,
                            "max_children":     -2,
                            "valid_children":  [ "folder" ],
                            "types": {
                                "default": {
                                    "valid_children": "none",
                                    "icon": {
                                        "image": "/images/document.png"
                                    }
                                },
                                "folder": {
                                    "valid_children": [ "default", "folder" ],
                                    "icon": {
                                        "image": "/images/folder.png"
                                    }
                                }
                            }
                        }
                    })
                    .bind("select_node.jstree", function (event, data) {
                        window.location = "edit.php?id=" + data.rslt.obj.attr("id");
                    })
                    .delegate("a", "click", function (event, data) { event.preventDefault(); });
                }
                $(document).ready(function(){
                    initTrees();

                });
            </script>

        </head>
        <body>

            <ul id="tree">
            </ul>

            <table border="1" id="properties"></table>

            <hr/>

            {% block content %}
                Hello {{ name }}!
            {% endblock %}
        </body>
    </html>