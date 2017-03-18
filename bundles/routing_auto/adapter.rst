.. index::
    single: Adapter; RoutingAutoBundle

Adapter
=======

Adapters will abstract all persistence operations. PhpcrOdmAdapter is available
to be use it with Doctrine PHPCR-ODM persistence layer by default. If you want
to use another persistence layer, you should implement your own adapter.

Implementing a custom adapter
-----------------------------

Adapter have to implement ``Symfony\Cmf\Component\RoutingAuto\AdapterInterface``::

    // src/AppBundle/Adapter/CustomAdapter.php
    namespace AppBundle\Adapter;

    use Symfony\Cmf\Component\RoutingAuto\AdapterInterface;

    class OrmAdapter implements AdapterInterface
    {
        // ... Implement all inherit methods
    }

The new adapter should be registered as services and tagged
with ``cmf_routing_auto.adapter`` and an ``alias`` that you have to set into
``adapter`` configuration:

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

        $definition = new Definition('AppBundle\RoutingAuto\CustomAdapter');
        $definition->addTag('cmf_routing_auto.adapter', array('alias' => 'custom_adapter'));

        $container->setDefinition('app.custom_adapter', $definition);

Using the new adapter:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing_auto:
            adapter: custom_adapter

    .. code-block:: xml

        .. todo

    .. code-block:: php

        $container->loadFromExtension('cmf_routing_auto', array(
            'adapter' => 'custom_adapter',
        ));

