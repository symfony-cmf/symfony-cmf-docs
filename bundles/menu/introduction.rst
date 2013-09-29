.. index::
    single: Menu; Bundles
    single: MenuBundle

MenuBundle
==========

The CmfMenuBundle extends the `KnpMenuBundle`_ and adds the following
features:

* Render menus stored in the persistance layer;
* Generate menu node URLs from linked Content or Route.

Note that only the Doctrine PHPCR-ODM persistance layer is supported in the
1.0 release.

.. caution::

    Make sure you understand how the `KnpMenuBundle`_ works when you want to
    customize the CmfMenuBundle. The CMF menu bundle is just adding
    functionality on top of the Knp menu bundle and this documentation is
    focused on the additional functionality.

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/menu-bundle`` package.

Dependencies
------------

This bundle extends the `KnpMenuBundle`_. Unless you change defaults and
provide your own implementations, this bundle also depends on:

* :doc:`CmfRoutingBundle <../routing/introduction>`- if you want to generate routes for content objects.
  Note that you need to explicitly enable the dynamic router as per default it
  is not loaded. See the
* :doc:`PHPCR-ODM <../phpcr_odm>` - to load route documents from the content
  repository when using the ``PhpcrMenuProvider``.

Creating a Simple Persistant Menu
---------------------------------

A menu created using the KnpMenuBundle is made up of a heierachy of class
instances implementing ``NodeInterface``. This is also true of a menu created
using MenuBundle documents.

It is recommended that the root document of the menu tree is a ``Menu``
document, all descendant documents should be ``MenuNode`` instances.

The root document should be a child of the document specified in the configuration
by the parameter ``persistence.phpcr.menu_basepath``, which defaults to ``/cms/menu``. Note
that if this document does not exist it must be created.

The example below creates a new menu with two items, "Home" and "Contact" and
we specify a URI for each::

    <?php

    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode;
    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu;
    use PHPCR\Util\NodeHelper;

    // create the menu root (assuming it doesn't already exist)
    NodeHelper::createPath('/cms/menu');
    $menuParent = $dm->find('/cms/menu');

    $menu = new Menu;
    $menu->setName('main-menu');
    $menu->setLabel('Main Menu');
    $menu->setParent($menuParent);
    $manager->persist($menu);

    $home = new MenuNode;
    $home->setName('home');
    $home->setLabel('Home');
    $home->setParent($menu);
    $home->setUri('http://www.example.com/home');
    $manager->persist($home);

    $contact = new MenuNode;
    $contact->setName('contact');
    $contact->setLabel('Contact');
    $contact->setParent($menu);
    $contact->setUri('http://www.example.com/contact');
    $manager->persist($contact);

    $manager->flush();

Rendering Menus
---------------

You render menus in the same way you would with the `KnpMenuBundle`_. The name
of the menu will correspond to the name of the root document in your menu
tree:

.. code-block:: jinja

    {{ knp_menu_render('main-menu') }}

Here we have specified the ``main-menu`` document from the previous
example. This will render an unordered list as follows:

.. code-block:: html

    <ul>
        <li class="first">
          <a href="http://www.example.com/home">Home</a>
        </li>
        <li class="last">
          <a href="http://www.example.com/contact">Contact</a>
        </li>
    </ul>

For more information see the `rendering menus`_ section of the KnpMenuBundle documentation.

Menu Documents
--------------

In accordance with the :ref:`CMF bundle standards
<contrib_bundles_baseandstandardimplementations>` you are provided with two
menu node implementations, a base document and a standard document.

Base Menu Node
~~~~~~~~~~~~~~

The ``MenuNodeBase`` document implements only those features available in the original KnpMenu node.

.. code-block:: php

    <?php

    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNodeBase;

    $parent = // get parent node

    // ODM specific
    $node = new MenuNodeBase;
    $node->setParent($parent);
    $node->setName('home');

    // Attributes are the HTML attributes of the DOM element representing the
    // menu node (e.g. <li/>)
    $node->setAttribute('attr_name', 'attr_value');
    $node->setAttributes(array('attr_name', 'attr_value'));
    $node->setChildrenAttributes(array('attr_name', 'attr_value'));

    // Display the node or not
    $node->setDisplay(true);
    $node->setDisplayChildren(true);

    // Any extra attributes you wish to associate with the menu node
    $node->setExtras(array('extra_param_1' => 'value'));

    // The label and the HTML attributes of the label
    $node->setLabel('Menu Node');
    $node->setLabelAttributes(array('style' => 'color: red;'));

    // The HTML attributes of the link (i.e. <a href=.../>
    $node->setLinkAttributes(array('style' => 'color: yellow;'));

    // Associate an implementation of Symfony\Component\Routing\RouteInterface
    $node->setRoute($route);
    // Specify if the route should be rendered absolute (otherwise relative)
    $node->setRouteAbsolute(true);
    // Parameters of the route
    $node->setRouteParameters(array());    

    // Specify a URI
    $node->setUri('http://www.example.com');

The Standard Menu Node
~~~~~~~~~~~~~~~~~~~~~~

The standard menu node adds the following CMF specific features:

* Ability to specify a link type (URI, route or content)
* Standard :ref:`publish workflow <bundle-core-publish_workflow>` integration;
* Ability to specify content as a link type.
* Translation.

Publish Workflow
""""""""""""""""

The standard menu node implements ``PublishTimePeriodInterface`` and
``PublishableInterface``. Please refer to the :ref:`publish workflow documentation <bundle-core-publish_workflow>`.

Menu Provider
~~~~~~~~~~~~~

A menu provider is responsible to load a menu when it is requested. KnpMenu
supports having several providers. The CmfMenuBundle provides the
``PhpcrMenuProvider`` to load menu items from PHPCR-ODM.

Every menu has a name and is loaded by that name. The ``PhpcrMenuProvider``
locates menus by looking at ``persistence.phpcr.menu_basepath``/``<menuname>``.
You can use custom document classes for menu nodes if needed, as long as they
implement ``Knp\Menu\NodeInterface`` to integrate with KnpMenuBundle. The
default ``MenuNode`` class discards children that do not implement the
``Knp\Menu\NodeInterface``.

.. note::

    There is currently no support for Doctrine ORM or other persistence
    managers. This is not by design, but only because nobody built that yet.
    We would be glad for a pull request refactoring ``PhpcrMenuProvider`` into
    a base class suitable for all doctrine implementations, and storage
    specific providers.

You can also write your completely custom provider and register it with the
KnpMenu as explained in the `KnpMenuBundle custom provider documentation`_.

Menu Factory
~~~~~~~~~~~~

The menu nodes need to be converted into menu items. This is the job of the
menu factory. A menu item should have a URL associated with it. KnpMenu can
either take the ``uri`` field from the options, or generate a URL from the
``route`` and ``routeParameters`` options. The CmfMenuBundle provides the
``ContentAwareFactory`` that supports to generate the URL from the ``content``
option that contains an object the ``DynamicRouter`` can generate a URL for, plus
eventual ``routeParameters``. Thus a menu node can link to a content object or
a route object in the database and put that object into the options to have the
URL generated.

URL generation is absolute or relative, depending on ``routeAbsolute``.
If you specify the ``linkType`` option, you can control how the URL is
generated. If this parameter is missing, it is determined automatically,
tacking in order ``uri``,``route`` or ``content``.

.. note::

    If you just want to generate normal Symfony routes with a menu that is in
    the database, simply make sure to never provide a ``content`` option and
    either provide the ``route`` and eventual ``routeParameters`` or the
    ``uri``.

Examples::

    <?php

    // get document manager
    $dm = ...;
    $rootNode = $dm->find(null, ...); // retrieve parent menu item

    // using referenceable content document
    $blogContent = $dm->find(null, '/my/cms/content/blog');

    $blogNode = new MenuNode();
    $blogNode->setName('blog');
    $blogNode->setParent($parent);
    $blogNode->setContent($blogDocument);
    $blogNode->setLabel('Blog');

    $dm->persist($blogNode);

    // using a route document
    $timelineRoute = $dm->find(null, '/my/cms/routes/timeline');

    $timelineNode = new MenuNode();
    $timelineNode->setContent($timelineRoute);
    // ...

    $dm->persist($timelineNode);

    // using a symfony route
    $sfRouteNode = new MenuNode();
    $sfRouteNode->setRoute('my_hard_coded_symfony_route');
    // ...

    $dm->persist($sfRouteNode);

    // using URL
    $urlNode = new MenuNode();
    $urlNode->setUri('http://www.example.com');
    // ...

    $dm->persist($urlNode);

    $dm->flush();

The default PHPCR-ODM mapping links content documents by a **weak** reference,
which means you are able to delete the referenced content.

Current Menu Item
-----------------

A menu item can be the current item. If it is the current item, this
information is passed to the twig template which by default adds the css class
``current`` and all menu items that are ancestors of that item get the class
``current_ancestor``. This will typically used in CSS to highlight menu
entries.

The decision about being current item happens by comparing the URI associated
with the menu item with the request URI. Additionally, the CMF menu bundle
supports voters that can look at the ``MenuItem`` and optionally the request.

There are two voter services configured but not enabled by default, another
voter that you can use to configure services and you can write your own voter
classes.

.. note::

    The CMF MenuBundle is based on Knp Menu 1.x. The 2.0 rewrite of Knp Menu
    will add current item voters in the core Knp library.  The CMF bundle
    voters are interface compatible and follow the same mechanism as Knp Menu
    to ease upgrading.

RequestContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter looks at the ``content`` field of the menu item extras and compares
it with the main content attribute of the request. The name for the main
content attribute in the request is configurable with the ``content_key``
option - if not set it defaults to the constant ``DynamicRouter::CONTENT_KEY``.

You can enable this voter by setting ``cmf_menu.voters.content_identity``
to ``~`` in your config.yml to use a custom ``content_key`` for the main
content attribute name, set it explicitly:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            voters:
                content_identity:
                    content_key: myKey

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voter>
                    <content-identity>
                        <content-key>myKey</content-key>
                    </content-identity>
                </voter>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'voters' => array(
                'content_identity' => array(
                    'content_key' => 'myKey',
                ),
            ),
        ));

UriPrefixVoter
~~~~~~~~~~~~~~

The uri prefix voter looks at the ``content`` field of the menu item extras and
checks if it contains an instance of the symfony Route class. If so, it
compares the route option ``currentUriPrefix`` with the request URI. This
allows you to make a whole sub-path of your site trigger the same menu item as
current, but you need to configure the prefix option on your route documents.

To enable the prefix voter, set the configuration key
``cmf_menu.voters.uri_prefix: ~``.

RequestParentContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter has the same logic of looking for a request attribute to get the
current content, but calls ``getParent`` on it. This parent is compared to the
``content`` of the menu item extras. That way, content that does not have its
own menu entry but a parent that does have a menu item can trigger the right
menu entry to be highlighted.

To use this voter you need to configure a custom service with the name of the
content in the request and your model class to avoid calling getParent on
objects that do not have that method.  You need to tag the service as
``cmf_menu.voter`` and also as ``cmf_request_aware`` because it
depends on the request. The service looks the same as for complete custom
voters (see below), except you do not need to write your own PHP code:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_bundle.menu_voter.parent:
                class: Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter
                arguments:
                    - contentDocument
                    - %my_bundle.my_model_class%
                tags:
                    - { name: "cmf_menu.voter" }
                    - { name: "cmf_request_aware" }

    .. code-block:: xml

        <service id="my_bundle.menu_voter.parent"
                 class="Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter">
            <argument>contentDocument</argument>
            <argument>%my_bundle.my_model_class%</argument>
            <tag name="cmf_menu.voter"/>
            <tag name="cmf_request_aware"/>
        </service>

    .. code-block:: php

        $definition = new Definition(
            'Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter',
            array('contentDocument', '%my_bundle.my_model_class%')
        ));
        $definition->addTag('cmf_menu.voter');
        $definition->addTag('cmf_request_aware');
        $container->setDefinition('my_bundle.menu_voter.parent', $definition);

Custom Voter
~~~~~~~~~~~~

Voters must implement the ``Symfony\Cmf\MenuBundle\Voter\VoterInterface``. To
make the menu bundle notice the voter, tag it with ``cmf_menu.voter``.
If the voter needs the request, add the tag ``cmf_request_aware`` to have a
listener calling ``setRequest`` on the voter before it votes for the first
time.

If you need to know the content the menu item points to, look in the
``content`` field of the menu item extras: ``$item->getExtra('content');``.
The ``ContentAwareFactory`` places the content referenced by the route there -
if it does reference a content. Your voter should handle the case where the
content is null.

For an example service definition see the section above for
``RequestParentIdentityVoter``.

A voter will look something like this::

    <?php
    namespace Acme\MenuBundle\Voter;

    use Symfony\Cmf\Bundle\MenuBundle\Voter\VoterInterface;
    use Knp\Menu\ItemInterface;

    class MyVoter implements VoterInterface
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }

        /**
         * {@inheritDoc}
         */
        public function matchItem(ItemInterface $item)
        {
            if (...) {
                // $item is the current menu item
                return true;
            }
            if (...) {
                // $item for sure is NOT the current menu item
                // even if other voters might match
                return false;
            }

            // we dont know if this is the current item
            return null;
        }
    }

Publish Workflow Interface
--------------------------

Menu nodes implement the write interfaces for publishable and publish time
period, see the :ref:`publish workflow documentation <bundle-core-publish-workflow>`
for more information.

.. _`KnpMenu`: https://github.com/knplabs/KnpMenu
.. _`KnpMenuBundle`: https://github.com/knplabs/KnpMenuBundle
.. _`KnpMenuBundle custom provider documentation`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/custom_provider.md

.. _`with composer`: http://getcomposer.org
.. _`rendering menus`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md#rendering-menus
