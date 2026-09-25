import { defineConfig } from 'vite';

export default defineConfig({
	plugins: [],
	server: {
		proxy: {
			'/ws': {
				target: 'ws://127.0.0.1:8765',
				ws: true,
			},
		},
	},
});
