**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use. ``null`` tells the manager registry to
retrieve the default manager.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_name``.
