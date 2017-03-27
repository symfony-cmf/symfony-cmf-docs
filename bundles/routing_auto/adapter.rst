.. index::
    single: Adapter; RoutingAutoBundle

Adapter
=======

Adapters abstract persistence operations. The ``PhpcrOdmAdapter`` is available
for the Doctrine PHPCR-ODM persistence layer. If you want to use a different
persistence layer, you need to implement your own adapter.

Implementing a custom adapter
-----------------------------

Adapters have to implement ``Symfony\Cmf\Component\RoutingAuto\AdapterInterface``::

    // src/AppBundle/Adapter/CustomAdapter.php
    namespace AppBundle\Adapter;

    use Symfony\Cmf\Component\RoutingAuto\AdapterInterface;

    class OrmAdapter implements AdapterInterface
    {
        // ... Implement all methods defined by the interface
    }

Adapters need to be registered as services and tagged with
``cmf_routing_auto.adapter`` and an ``alias`` to identify the adapter:

.. configuration-block::

    .. code-block:: yaml

        services:
            app.custom_adapter:
                class: AppBundle\RoutingAuto\CustomAdapter
                tags:
                    - { name: cmf_routing_auto.adapter, alias: "custom_adapter"}

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="app.custom_adapter"
                class="AppBundle\RoutingAuto\CustomAdapter"
            >
                <tag name="cmf_routing_auto.adapter" alias="custom_adapter"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use AppBundle\RoutingAuto\CustomAdapter;

        $definition = new Definition(CustomAdapter::class);
        $definition->addTag('cmf_routing_auto.adapter', array('alias' => 'custom_adapter'));

        $container->setDefinition('app.custom_adapter', $definition);

To use the new adapter, you specify the alias in the routing auto configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing_auto:
            adapter: custom_adapter

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routingauto"
                adapter="custom_adapter"
            />
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing_auto', array(
            'adapter' => 'custom_adapter',
        ));
