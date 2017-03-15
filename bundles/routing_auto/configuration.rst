Configuration Reference
=======================

The RoutingAutoBundle can be configured under the ``cmf_routing_auto`` key in your
application configuration.

Configuration
-------------

``adapter``
~~~~~~~~~~~

**type**: ``scalar`` **default**: ``doctrine_phpcr_odm`` if ``persistence`` configuration option is set ``phpcr``

This define the adapter used to manage routes.

To be able to use another persistence implementation you have to create your own
adapter by implementing ``Symfony\Cmf\Component\RoutingAuto\AdapterInterface``
and create it as a service, ``tag`` it with: ``cmf_routing_auto.adapter`` name and
an ``alias`` that you have to set into ``adapter`` configuration:

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

    .. code-block:: php

        $container->loadFromExtension('cmf_routing_auto', array(
            'adapter' => 'custom_adapter',
        ));


``auto_mapping``
~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``true``

Look for the configuration file ``cmf_routing_auto.yml`` in `Resource/config` folder of all
available bundles.

``persistence``
~~~~~~~~~~~~~~~

``phpcr``
.........