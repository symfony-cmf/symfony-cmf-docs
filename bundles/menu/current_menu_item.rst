.. _bundles_menu_current_menu_item:

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

