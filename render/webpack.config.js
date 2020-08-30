const path = require("path");

module.exports = {
    entry: "./src/index.ts",
    mode: "production",
    output: {
        path: path.resolve(__dirname, "../bin/resources/"),
        filename: "bundle.js"
    },
    resolve: {
        extensions: [".ts", ".tsx"] 
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: [
                    {
                        loader: 'ts-loader',
                        options: {
                            configFile: path.resolve(__dirname, './tsconfig.json'),
                        }
                    }
                ]
            }
        ]
    },
};