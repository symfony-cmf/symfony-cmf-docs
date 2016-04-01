.. index::
    single: Definitions; RoutingAutoBundle

Multiple Auto Routes
====================

.. versionadded: 2.0

    The capability to add multiple routes for a managed object has been
    introduced in RoutingAutoBundle 2.0.

In the introduction you were shown an example using a single auto route schema
definition. It is possible to have multiple auto-route definitions for each of your
managed objects, and therefore multiple routes.

Below we will modify the example given in the :doc:`Introduction
<introduction>` and add a
new ``edit`` schema definition which has the ``/admin`` prefix:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            definitions:
                 main:
                     uri_schema: /my-forum/{category}/{title}
                     defaults:
                         type: view
                 edit:
                     uri_schema: /admin/my-forum/{category}/{title}
                     defaults:
                         type: edit
            token_providers:
                category: [content_method, { method: getCategoryTitle, slugify: true }]
                title: [content_method, { method: getTitle }] # slugify is true by default

    .. code-block:: xml

        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <?xml version="1.0" ?>
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic">
                <definition name="main" uri-schema="/my-forum/{category}/{title}">
                    <default key="type">view</default>
                </definition>

                <definition name="edit" uri-schema="/admin/my-forum/{category}/{title}">
                    <default key="type">edit</default>
                </definition>

                <token-provider token="category" name="content_method">
                    <option name="method">getCategoryName</option>
                    <option name="slugify">true</option>
                </token-provider>

                <token-provider token="title" name="content_method">
                    <option name="method">getTitle</option>
                </token-provider>
            </mapping>
        </auto-mapping>

Note that we specify the ``defaults``. The ``defaults`` key can be used to
persist key / value information on a route. This information can later be used
for many things, but importantly it can be used by the :doc:`Dynamic Router
<../routing/dynamic>` to determine which controller should be used to handle
the request.

For example, the dynamic router could be configured as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            # ...
            dynamic:
                # ...
                controllers_by_type:
                    view: Acme\\BasicCmsBundle\\Controller\\ViewController
                    edit: Acme\\BasicCmsBundle\\Controller\\ViewController

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <! -- ... -->
                <dynamic>
                    <! -- ... -->
                    <controller-by-type
                        type="view">
                        Acme\\BasicCmsBundle\\Controller\\ViewController
                    </controller-by-type>
                    <controller-by-type
                        type="edit">
                        Acme\\BasicCmsBundle\\Controller\\EditController
                    </controller-by-type>
                </dynamic>
            </config>
        </container>
