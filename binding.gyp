{
  "targets": [
    {
      "target_name": "winregistry",
      "conditions": [
        ['OS=="win"', {
          "sources": [
            "src/winregistry.cc"
          ]
        }]
      ],
			"include_dirs" : [ 
    		"<!(node -e \"require('nan')\")"
			]
    }
  ]
}
